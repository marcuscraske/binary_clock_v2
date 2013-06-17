-- ****************************** Create the schema for the clock *********************************************
CREATE SCHEMA bc;
SET search_path TO bc;

-- ****************************** Create the user for the clock to access ONLY the bc schema ******************
CREATE USER bc;
GRANT ALL PRIVILEGES ON SCHEMA bc TO bc;

-- ****************************** Create tables ***************************************************************
-- Location table: allows information to be associated with countries and reduce repeated data in tables
-- such as the ip-lookup table.
CREATE TABLE tb_locations
(
	locationid	INT		PRIMARY KEY,
	-- Textural representation of the location e.g. United Kingdom.
	name		VARCHAR(128)	NOT NULL,
	-- Stores the URL of the flag image used to represent the location.
	flag_img	VARCHAR(128)	DEFAULT './Content/Images/Flags/unknown.png'
);
-- -- The first tuple should be for unknown locations.
INSERT INTO tb_locations (locationid, name) VALUES(0, 'Unknown');

-- IP lookup table: reduces storage complexity for repeated IP addresses by using a single integer rather
-- than 45 characters each time. The table allows us to also associate information with IP addresses e.g.
-- country/location etc.
CREATE TABLE tb_ip_lookup
(
	ipid		INT		PRIMARY KEY,
	-- Support for IPv6 (max length of 45 if we include IPv4 tunneling) - future proof (for now)
	ip		VARCHAR(45)	NOT NULL,
	locationid	INT		CONSTRAINT fk_iplookup_loc_fk		REFERENCES	bc.tb_locations(locationid)	ON DELETE CASCADE ON UPDATE CASCADE DEFAULT 0
);

-- -- Guestbook: stores the posts of users in the guestbook; posts can be the child of another post, however
-- -- there may only be a maximum of three sub-trees.
CREATE TABLE tb_guestbook
(
	postid 		INT 		PRIMARY KEY,
	parent_postid	INT		CONSTRAINT fk_guestbook_post_fk		REFERENCES	bc.tb_guestbook(postid)		ON DELETE CASCADE ON UPDATE CASCADE,
	ipid		INT		CONSTRAINT fk_guestbook_ip_fk		REFERENCES	bc.tb_ip_lookup(ipid)		ON DELETE CASCADE ON UPDATE CASCADE NOT NULL,
	-- The user's post in the guestbook.
	message		TEXT		NOT NULL
	-- Checks the post has less than three children above it.
	-- CHECK SELECT guestbook_tree_count(parent_postid) < 3
);

-- -- Bans: used for banning IP ranges
CREATE TABLE tb_ipbans
(
	banid		INT		PRIMARY KEY,
	ip_start	VARCHAR(45)	NOT NULL,
	ip_end		VARCHAR(45)	NOT NULL
);
