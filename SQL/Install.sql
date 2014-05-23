-- ****************************** Create the schema for the clock *********************************************
CREATE SCHEMA bc;
SET search_path TO bc;

-- ****************************** Create the user for the clock to access ONLY the bc schema ******************
CREATE USER bc;
GRANT ALL PRIVILEGES ON SCHEMA bc TO bc;

-- ****************************** Setup PGSQL language ********************************************************
--CREATE LANGUAGE plpgsql;

-- ****************************** Create tables ***************************************************************
-- Location table: allows information to be associated with countries and reduce repeated data in tables
-- such as the ip-lookup table.
CREATE TABLE bc.tb_locations
(
	locationid	INT		PRIMARY KEY,
	-- Textural representation of the location e.g. United Kingdom.
	name		VARCHAR(128)	NOT NULL,
	-- Stores the URL of the flag image used to represent the location.
	flag_img	VARCHAR(128)	DEFAULT '/Content/Images/Flags/unknown.png'
);
-- -- The first tuple should be for unknown locations.
INSERT INTO bc.tb_locations (locationid, name) VALUES(0, 'Unknown Country');
INSERT INTO bc.tb_locations (locationid, name) VALUES(1, 'Unknown IP Block Allocation');

-- IP lookup table: reduces storage complexity for repeated IP addresses by using a single integer rather
-- than 45 characters each time. The table allows us to also associate information with IP addresses e.g.
-- country/location etc.
CREATE TABLE bc.tb_ip_lookup
(
	ipid		INT		PRIMARY KEY,
	-- Support for IPv6 (max length of 45 if we include IPv4 tunneling) - future proof (for now)
	ip		VARCHAR(45)	NOT NULL 	UNIQUE,
	locationid	INT		CONSTRAINT fk_iplookup_loc_fk		REFERENCES	bc.tb_locations(locationid)	ON DELETE CASCADE ON UPDATE CASCADE DEFAULT 0
);

-- -- Guestbook: stores the posts of users in the guestbook; posts can be the child of another post, however
-- -- there may only be a maximum of three sub-trees.
CREATE TABLE bc.tb_guestbook
(
	postid 		INT 		PRIMARY KEY,
	parent_postid	INT		CONSTRAINT fk_guestbook_post_fk		REFERENCES	bc.tb_guestbook(postid)		ON DELETE CASCADE ON UPDATE CASCADE,
	ipid		INT		CONSTRAINT fk_guestbook_ip_fk		REFERENCES	bc.tb_ip_lookup(ipid)		ON DELETE CASCADE ON UPDATE CASCADE NOT NULL,
	name		VARCHAR(24),
	message		TEXT		NOT NULL,
	datetime	TIMESTAMP	NOT NULL,
	-- Stores the number of subposts for recursively displaying sub-posts - saves lots of expensive queries.
	subposts	INT		DEFAULT 0
	-- Checks the post has less than three children above it.
	-- CHECK SELECT guestbook_tree_count(parent_postid) < 3
);

-- -- Bans: used for banning IP ranges
CREATE TABLE bc.tb_ipbans
(
	banid		INT		PRIMARY KEY,
	ip		VARCHAR(45)	NOT NULL
);

-- ****************************** Functions *****************************************************************
CREATE OR REPLACE FUNCTION bc.func_date(datetime TIMESTAMP)
	RETURNS TEXT AS
$$
DECLARE
	temp INTEGER;
BEGIN
	-- Check the date is within 24hrs, else display the date
	IF datetime > current_timestamp - INTERVAL '1 minute' THEN
		temp = CAST(EXTRACT(second FROM (current_timestamp - datetime)) AS INTEGER);
		IF temp > 1 THEN
			RETURN CONCAT(temp, ' seconds ago');
		ELSE
			RETURN '1 second ago';
		END IF;
	ELSEIF datetime > current_timestamp - INTERVAL '1 hours' THEN
		temp = CAST(EXTRACT(minute FROM (current_timestamp - datetime)) AS INTEGER);
		IF temp > 1 THEN
			RETURN CONCAT(temp, ' minutes ago');
		ELSE
			RETURN '1 minute ago';
		END IF;
	ELSEIF datetime > current_timestamp - INTERVAL '1 days' THEN
		temp = CAST(EXTRACT(hour FROM (current_timestamp - datetime)) AS INTEGER);
		IF temp > 1 THEN
			RETURN CONCAT(temp, ' hours ago');
		ELSE
			RETURN '1 hour ago';
		END IF;
	ELSE
		RETURN to_char(datetime, 'DD"/"MM"/"YYYY');
	END IF;
END;
$$
LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION bc.func_gb_subposts_cache()
	RETURNS TRIGGER AS
$$
BEGIN
	IF TG_OP = 'DELETE' THEN
		-- Destroy children
		DELETE FROM bc.tb_guestbook WHERE parent_postid=OLD.postid;
		-- Update parent's subpost count
		UPDATE bc.tb_guestbook SET subposts=(SELECT COUNT('') FROM bc.tb_guestbook WHERE parent_postid=OLD.parent_postid) WHERE postid=OLD.parent_postid;
		RETURN OLD;	
	ELSE
		-- Update parent's count
		IF TG_OP = 'UPDATE' AND OLD.parent_postid != NEW.parent_postid THEN
			UPDATE bc.tb_guestbook SET subposts=(SELECT COUNT('') FROM bc.tb_guestbook WHERE parent_postid=OLD.parent_postid) WHERE postid=OLD.parent_postid;
		END IF;
		UPDATE bc.tb_guestbook SET subposts=(SELECT COUNT('') FROM bc.tb_guestbook WHERE parent_postid=NEW.parent_postid) WHERE postid=NEW.parent_postid;
		RETURN NEW;	
	END IF;
END;
$$
LANGUAGE PLPGSQL;

-- ****************************** Triggers ******************************************************************
CREATE TRIGGER trig_gb_subpostss_cache AFTER INSERT OR UPDATE OR DELETE ON bc.tb_guestbook
	FOR EACH ROW
		EXECUTE PROCEDURE func_gb_subposts_cache();
		

-- ****************************** Views *********************************************************************
CREATE OR REPLACE VIEW bc.vi_guestbook_posts AS
	SELECT gb.postid, gb.parent_postid, gb.subposts, gb.message, gb.datetime, to_char(gb.datetime, 'DD-MM-YYYY "("HH24:MI:SS")"') AS datetime_raw, bc.func_date(gb.datetime) AS datetime_formatted, gb.name AS name, l.name AS cname, l.flag_img, ip.ip, ip.ipid FROM bc.tb_guestbook AS gb LEFT OUTER JOIN bc.tb_ip_lookup AS ip ON ip.ipid=gb.ipid LEFT OUTER JOIN bc.tb_locations AS l ON l.locationid=ip.locationid ORDER BY gb.datetime DESC;

-- ****************************** Data ***********************************************************************
INSERT INTO bc.tb_locations (locationid, name, flag_img) VALUES
(2,'CA','/Content/Images/Flags/CA.png'),
(3,'DE','/Content/Images/Flags/DE.png'),
(4,'FR','/Content/Images/Flags/FR.png'),
(5,'GB','/Content/Images/Flags/GB.png'),
(6,'IE','/Content/Images/Flags/IE.png')
;

