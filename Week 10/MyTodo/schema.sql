CREATE TABLE users (
	id			INTEGER,
	username	TEXT NOT NULL,
	hash		TEXT NOT NULL,
	email		TEXT NOT NULL,
	created		TEXT NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE tasks (
	id			INTEGER,
	title		TEXT NOT NULL,
	desc		TEXT,
	created		TEXT NOT NULL,
	due			TEXT,
	isdone		INTEGER NOT NULL DEFAULT 0,
	isdeleted	INTEGER NOT NULL DEFAULT 0,
	userid		INTEGER NOT NULL,
	FOREIGN KEY(userid) REFERENCES users(id),	
	PRIMARY KEY(id)
);