SET client_min_messages = 'warning';


---------------------------------------------------------------------
----------------------- Удаление таблиц -----------------------------

-- Порядок должен быть строго обратным порядку, 
-- в котором таблицы создаются.

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
DROP TABLE IF EXISTS Plugin;
DROP TABLE IF EXISTS MUser;



---------------------------------------------------------------------
----------------------- Создание табьиц -----------------------------

CREATE TABLE MUser (
	id 					SERIAL PRIMARY KEY,
	login 				VARCHAR,
	password 			VARCHAR,
	salt 				VARCHAR,
	is_admin 			BOOL
);

CREATE TABLE Plugin (
	id 					SERIAL PRIMARY KEY,
	textid 				VARCHAR,
	name 				VARCHAR
);

CREATE TABLE UserPluginAccess (
	id 					SERIAL PRIMARY KEY,
	userid 				INTEGER REFERENCES MUser (id),
	pluginid 			INTEGER REFERENCES Plugin (id)
);



CREATE TABLE Patient (
	id 					SERIAL PRIMARY KEY,
	familyName 			VARCHAR,
	name 				VARCHAR,
	patronymic 			VARCHAR,
	birthDay			TIMESTAMP
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
	id 					INTEGER PRIMARY KEY,
	name				VARCHAR NOT NULL,
	shortName			VARCHAR,
	inn					CHAR(12),
	kpp					CHAR(9),
	okonh				CHAR(5),
	okato				CHAR(11), -- TODO: 11 или 8?
	okpo				CHAR(10),
	ogrn				CHAR(13)
);
-- Следующие два правила не позволяют изменять количество записей, если 
-- в таблице уже есть одна.
-- Ничего не делать, если при вставке записи id <> 1.
CREATE RULE HealthFacilityInsert AS ON INSERT TO HealthFacility 
WHERE id <> 1 DO INSTEAD NOTHING;
-- Ничего не делать, если при вставке записи id =  1.
CREATE RULE HealthFacilityDelete AS ON DELETE TO HealthFacility 
WHERE id = 1  DO INSTEAD NOTHING;


CREATE TABLE Staff (
	id					SERIAL PRIMARY KEY,
	familyName 			VARCHAR,
	name 				VARCHAR,
	patronymic 			VARCHAR,
	birthDay			TIMESTAMP,
	specialization		VARCHAR
);


CREATE TABLE DepartmentType (
	id					SERIAL PRIMARY KEY,
	textid				VARCHAR,
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
----------------------- Данные --------------------------------------

-- Пользователь с правами админа с логином 'admin' и паролем 'pw'.
INSERT INTO MUser(login, password, salt, is_admin) VALUES
('admin', '38b311d8c359e5975c5a3f454d3f4294', 'salt', true);


INSERT INTO Plugin(textid, name) VALUES
('admin', 'Администратор'),
('patients', 'Пациенты'),
('mkb10', 'Справочная система МКБ-10'),
('departments', 'Медицинское учреждение');


INSERT INTO DocumentType(textid, name) VALUES
('passport', 'Паспорт'),
('inn', 'ИНН');


INSERT INTO HealthFacility(id, name) VALUES(1, 'Медицинское учреждение');


INSERT INTO DepartmentType(textid, name) VALUES
('clinic', 'Амбулатория'),
('hospital', 'Стационар');


---------------------------------------------------------------------
SET client_min_messages = 'notice';
