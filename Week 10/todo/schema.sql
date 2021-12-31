CREATE TABLE users (
	id		INTEGER,
	username	TEXT NOT NULL,
	hash		TEXT NOT NULL,
	email		TEXT NOT NULL,
	timestamp	TEXT NOT NULL,
	PRIMARY KEY(id))