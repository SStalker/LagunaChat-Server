DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS user_friends;
DROP TABLE IF EXISTS added_users;
DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS user_deny_messages;
DROP TABLE IF EXISTS user_authorize_messages;


CREATE TABLE users(
	UID INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	email VARCHAR(50) NOT NULL UNIQUE,
	password CHAR(32) NOT NULL,
	firstname VARCHAR(32),
	lastname VARCHAR(32),
	birthday CHAR(11),
	online TINYINT(1) NOT NULL
);

CREATE TABLE user_friends(
	UID INT NOT NULL,
	FID INT NOT NULL,
	PRIMARY KEY(UID,FID)
);

CREATE TABLE added_users(
	AUID INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	from_user VARCHAR(50) NOT NULL,
	to_user VARCHAR(50) NOT NULL,
	transmitted TINYINT(1) NOT NULL
);

CREATE TABLE messages(
	MID INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	from_user VARCHAR(50) NOT NULL,
	to_user VARCHAR(50) NOT NULL,
	message text NOT NULL,
	transmitted TINYINT(1) NOT NULL,
	arrived TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


CREATE TABLE user_deny_messages(
	UDMID INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	from_user VARCHAR(50) NOT NULL,
	to_user VARCHAR(50) NOT NULL,
	transmitted TINYINT(1) NOT NULL
);

CREATE TABLE user_authorize_messages(
	UAMID INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	from_user VARCHAR(50) NOT NULL,
	to_user VARCHAR(50) NOT NULL,
	transmitted TINYINT(1) NOT NULL
);

INSERT INTO
users
VALUES(0,"test@test.de","598d4c200461b81522a3328565c25f7c","STalker","","1989-07.11",0);


INSERT INTO
users
VALUES(0,"test@test.org","598d4c200461b81522a3328565c25f7c","Tester2","","11.07.1989",0);

INSERT INTO
users
VALUES(0,"test@test.com","598d4c200461b81522a3328565c25f7c","James","","11.07.1989",0);

INSERT INTO user_friends VALUES(1,2);
INSERT INTO user_friends VALUES(1,3);
INSERT INTO user_friends VALUES(2,1);
INSERT INTO user_friends VALUES(3,1);

SELECT u.email FROM users AS u,user_friends AS uf WHERE uf.uid=(SELECT uid FROM users WHERE email="test@test.de") AND u.uid = uf.fid
