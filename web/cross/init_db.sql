CREATE TABLE users(
	name TEXT NOT NULL UNIQUE,
	admin INT NOT NULL,
	password TEXT NOT NULL
	);

CREATE TABLE reports(
	author INT NOT NULL,
	msg TEXT NOT NULL,
	FOREIGN KEY(msg) REFERENCES users(rowid)
	);

INSERT INTO users VALUES('admin',1,'$2y$10$xiwWLn0xNYeL42KzaUB2eeY1iGk6sTaQozQQogdyaSdZFFvtoFR8a');
INSERT INTO reports VALUES(1,'CSSOC{90d_1_h473_W38D3V3L0pM3n7_4nd_PhP}');
