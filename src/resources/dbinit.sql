---------------------------------------------------------------------
----------------------- Dropping tables -----------------------------

DROP TABLE IF EXISTS Address;
DROP TABLE IF EXISTS Document;
DROP TABLE IF EXISTS DocumentType; 
DROP TABLE IF EXISTS Patient;
DROP TABLE IF EXISTS UserPluginAccess;
DROP TABLE IF EXISTS Plugins;
DROP TABLE IF EXISTS MUser;



---------------------------------------------------------------------
-------------------------- Creating tables --------------------------
CREATE TABLE MUser (
	id 					SERIAL PRIMARY KEY,
	login 				VARCHAR,
	password 			VARCHAR,
	salt 				VARCHAR,
	is_admin 			BOOL
);

CREATE TABLE Plugins (
	id 					SERIAL PRIMARY KEY,
	textid 				VARCHAR,
	name 				VARCHAR
);

CREATE TABLE UserPluginAccess (
	id 					SERIAL,
	userid 				INTEGER REFERENCES MUser (id),
	pluginid 			INTEGER REFERENCES Plugins (id)
);



CREATE TABLE Patient (
	id 					SERIAL PRIMARY KEY,
	familyName 			VARCHAR,
	name 				VARCHAR,
	patronymic 			VARCHAR
);

CREATE TABLE DocumentType (
	id 					SERIAL PRIMARY KEY, 
	textid 				VARCHAR, 
	name		 		VARCHAR
);

CREATE TABLE Document (
	id 					SERIAL PRIMARY KEY,
	documentTypeId 		INTEGER REFERENCES DocumentType (id),
	patientId 			INTEGER REFERENCES Patient (id),
	serialNumber 		VARCHAR,
	date 				TIMESTAMP,
	givenBy 			VARCHAR
);

CREATE TABLE Address (
	id 					SERIAL PRIMARY KEY,
	patientId 			INTEGER REFERENCES Patient (id),
	isMailingAddress 	BOOL NOT NULL,		-- Является адресом по прописке.
	city 				VARCHAR,
	street 				VARCHAR,
	house 				VARCHAR, 
	apartment 			VARCHAR
);


---------------------------------------------------------------------
----------------------------- Data ----------------------------------
-- Admin user with login 'admin' and password 'pw'.
INSERT INTO MUser(login, password, salt, is_admin) VALUES 
('admin', '38b311d8c359e5975c5a3f454d3f4294', 'salt', true);


INSERT INTO Plugins(textid, name) VALUES
('admin', 'Администратор'), 
('patients', 'Пациенты');


INSERT INTO DocumentType(textid, name) VALUES
('passport', 'Паспорт'), 
('taxpayerId', 'ИНН');
