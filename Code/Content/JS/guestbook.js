function gbReply(postid)
{
	document.getElementById("gb_parent").value = postid;
	location.href = "#";
	location.href = "#gb_reply";
	return false;
}
