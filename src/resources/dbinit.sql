SET client_min_messages = 'warning';


---------------------------------------------------------------------
----------------------- Dropping tables -----------------------------


DROP TABLE IF EXISTS DepartmentStaffPosition;
DROP TABLE IF EXISTS Position;
DROP TABLE IF EXISTS Department;
DROP TABLE IF EXISTS DepartmentType;
DROP TABLE IF EXISTS Staff;
DROP TABLE IF EXISTS HealthFacility;
DROP TABLE IF EXISTS MKB10;
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
	id 					SERIAL PRIMARY KEY,
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
	serialNumber 		VARCHAR NOT NULL,
	date 				TIMESTAMP NOT NULL,
	givenBy 			VARCHAR NOT NULL
);

CREATE TABLE Address (
	id 					SERIAL PRIMARY KEY,
	patientId 			INTEGER REFERENCES Patient (id),
	isMailingAddress 	BOOL NOT NULL,		-- Является адресом по прописке.
	city 				VARCHAR NOT NULL,
	street 				VARCHAR NOT NULL,
	house 				VARCHAR NOT NULL,
	apartment 			VARCHAR
);



CREATE TABLE MKB10 (
	id					INTEGER NOT NULL UNIQUE,
	description			VARCHAR,
	parentId			INTEGER REFERENCES MKB10(id)
);



CREATE TABLE HealthFacility (
	name				VARCHAR,
	shortName			VARCHAR

);


CREATE TABLE Staff (
	id					SERIAL PRIMARY KEY,
	familyName 			VARCHAR,
	name 				VARCHAR,
	patronymic 			VARCHAR,
	birthDate			TIMESTAMP,
	specialization		VARCHAR
);


CREATE TABLE DepartmentType (
	id					SERIAL PRIMARY KEY,
	name				VARCHAR
);


CREATE TABLE Department (
	id					SERIAL PRIMARY KEY,
	name				VARCHAR,
	shortName			VARCHAR,
	typeId				INTEGER REFERENCES DepartmentType(id),
	headOfDepartmentId	INTEGER REFERENCES Staff(id)
);


CREATE TABLE Position (
	id					SERIAL PRIMARY KEY,
	name				VARCHAR
);


CREATE TABLE DepartmentStaffPosition (
	departmentId		INTEGER REFERENCES Department(id),
	staffId				INTEGER REFERENCES Staff(id),
	positionId			INTEGER REFERENCES Position(id)
);


---------------------------------------------------------------------
----------------------------- Data ----------------------------------
-- Admin user with login 'admin' and password 'pw'.
INSERT INTO MUser(login, password, salt, is_admin) VALUES
('admin', '38b311d8c359e5975c5a3f454d3f4294', 'salt', true);


INSERT INTO Plugins(textid, name) VALUES
('admin', 'Администратор'),
('patients', 'Пациенты'),
('mkb10', 'Справочная система МКБ-10');


INSERT INTO DocumentType(textid, name) VALUES
('passport', 'Паспорт'),
('taxpayerId', 'ИНН');


---------------------------------------------------------------------
SET client_min_messages = 'notice';
