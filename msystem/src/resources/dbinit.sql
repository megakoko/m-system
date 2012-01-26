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
DROP TABLE IF EXISTS AddressType;
DROP TABLE IF EXISTS Document;
DROP TABLE IF EXISTS DocumentType;
DROP TABLE IF EXISTS Patient;
DROP TABLE IF EXISTS Sex;
DROP TABLE IF EXISTS UserPluginAccess;
DROP TABLE IF EXISTS Plugin;
DROP TABLE IF EXISTS MUser;



---------------------------------------------------------------------
----------------------- Создание табьиц -----------------------------

-- Таблица с пользователями.
CREATE TABLE MUser (
	id 					SERIAL PRIMARY KEY,
	login 				VARCHAR (20) NOT NULL,
	password 			VARCHAR (32),	-- Достаточно для MD5.
	salt 				VARCHAR (20),
	is_admin 			BOOL NOT NULL DEFAULT 'false'
);

-- Таблица, содержащая информацию о модулях.
CREATE TABLE Plugin (
	id 					SERIAL PRIMARY KEY,
	textid 				VARCHAR (30) NOT NULL UNIQUE,
	name 				VARCHAR (100) NOT NULL
);

-- Таблица, определяющая доступ пользователей к модулям.
CREATE TABLE UserPluginAccess (
	id 					SERIAL PRIMARY KEY,
	userid 				INTEGER REFERENCES MUser (id),
	pluginid 			INTEGER REFERENCES Plugin (id)
);


-- Пол пациентов.
CREATE TABLE Sex (
	id 					SERIAL PRIMARY KEY,
	textid				VARCHAR (30) NOT NULL UNIQUE,
	name				VARCHAR (40) NOT NULL
);


-- Пациенты.
CREATE TABLE Patient (
	id 					SERIAL PRIMARY KEY,
	familyName 			VARCHAR NOT NULL, -- TODO
	name 				VARCHAR NOT NULL,
	patronymic 			VARCHAR NOT NULL,
	birthDay			TIMESTAMP NOT NULL,
	sexId				INTEGER REFERENCES Sex(id)
);

-- Типы документа.
CREATE TABLE DocumentType (
	id 					SERIAL PRIMARY KEY,
	textid 				VARCHAR (30) NOT NULL UNIQUE,
	name		 		VARCHAR (40) NOT NULL
);

-- Документы.
CREATE TABLE Document (
	id 					SERIAL PRIMARY KEY,
	documentTypeId 		INTEGER REFERENCES DocumentType (id),
	patientId 			INTEGER REFERENCES Patient (id),
	serialNumber 		VARCHAR (30) NOT NULL,
	date 				TIMESTAMP NOT NULL,
	givenBy 			VARCHAR (100) NOT NULL
);


CREATE TABLE AddressType (
	id					SERIAL PRIMARY KEY,
	textid				VARCHAR (30) NOT NULL UNIQUE,
	name				VARCHAR (40) NOT NULL
);


-- Адреса.
CREATE TABLE Address (
	id 					SERIAL PRIMARY KEY,
	patientId 			INTEGER REFERENCES Patient (id),
	typeId				INTEGER REFERENCES AddressType (id),
	city 				VARCHAR (40) NOT NULL,
	street 				VARCHAR (100) NOT NULL,
	house 				VARCHAR (30) NOT NULL,
	apartment 			VARCHAR (30)
);



-- Таблица для справочной системы МКБ-10.
CREATE TABLE MKB10 (
	id					INTEGER PRIMARY KEY,
	description			VARCHAR NOT NULL,
	parentId			INTEGER REFERENCES MKB10(id)
);



-- Таблица с информацией о мед. учреждении.
CREATE TABLE HealthFacility (
	id 					INTEGER PRIMARY KEY,
	name				VARCHAR (100) NOT NULL,
	shortName			VARCHAR (30),
	city				VARCHAR (40),
	street				VARCHAR (100),
	house				VARCHAR (30),
	inn					CHAR(12),
	kpp					CHAR(9),
	okonh				CHAR(5),
	okato				CHAR(11), -- TODO: 11 или 8?
	okpo				CHAR(10),
	ogrn				CHAR(13)
);


-- Персонал/работники мед. учреждения.
CREATE TABLE Staff (
	id					SERIAL PRIMARY KEY,
	familyName 			VARCHAR (40) NOT NULL,
	name 				VARCHAR (40) NOT NULL,
	patronymic 			VARCHAR (40) NOT NULL,
	birthDay			TIMESTAMP NOT NULL,
	specialization		VARCHAR (100)
);


-- Типы отделений (стационар/амбулатория).
CREATE TABLE DepartmentType (
	id					SERIAL PRIMARY KEY,
	textid				VARCHAR (30) NOT NULL UNIQUE,
	name				VARCHAR (40) NOT NULL
);


-- Отделения мед. учреждения.
CREATE TABLE Department (
	id					SERIAL PRIMARY KEY,
	name				VARCHAR (100) NOT NULL,
	shortName			VARCHAR (30) NOT NULL,
	typeId				INTEGER REFERENCES DepartmentType(id),
	headOfDepartmentId	INTEGER REFERENCES Staff(id)
);


-- Должности.
CREATE TABLE Position (
	id					SERIAL PRIMARY KEY,
	name				VARCHAR (40) NOT NULL
);


-- Таблица, связывающая отделения, персонал и должности. Определяет, 
-- кто где работает на какой должности.
CREATE TABLE DepartmentStaffPosition (
	id					SERIAL PRIMARY KEY,
	departmentId		INTEGER REFERENCES Department(id),
	staffId				INTEGER REFERENCES Staff(id),
	positionId			INTEGER REFERENCES Position(id)
);


---------------------------------------------------------------------
----------------------- Данные --------------------------------------

-- Пользователь с правами админа с логином 'admin' и паролем 'pw'.
INSERT INTO MUser(login, password, salt, is_admin) VALUES
('admin', '38b311d8c359e5975c5a3f454d3f4294', 'salt', 'true');


-- При добавлении модуля надо добавлять сюда строку с его textid и названием.
INSERT INTO Plugin(textid, name) VALUES ('admin', 'Администратор');
INSERT INTO Plugin(textid, name) VALUES ('patients', 'Пациенты');
INSERT INTO Plugin(textid, name) VALUES ('mkb10', 'Справочная система МКБ-10');
INSERT INTO Plugin(textid, name) VALUES ('departments', 'Медицинское учреждение');
INSERT INTO Plugin(textid, name) VALUES ('test', 'Пример модуля');


INSERT INTO Sex(textid, name) VALUES ('male', 'Мужчина');
INSERT INTO Sex(textid, name) VALUES ('female', 'Женщина');


INSERT INTO DocumentType(textid, name) VALUES ('passport', 'Паспорт');
INSERT INTO DocumentType(textid, name) VALUES ('inn', 'ИНН');
INSERT INTO DocumentType(textid, name) VALUES ('insuranceVoluntary', 'Полис добровольного страхования');
INSERT INTO DocumentType(textid, name) VALUES ('insuranceMandatory', 'Полис обязательного страхования');


INSERT INTO AddressType(textid, name) VALUES ('mailing', 'Адрес прописки');
INSERT INTO AddressType(textid, name) VALUES ('actual', 'Адрес проживания');


INSERT INTO HealthFacility(id, name) VALUES(1, 'Медицинское учреждение');


INSERT INTO DepartmentType(textid, name) VALUES ('clinic', 'Амбулатория');
INSERT INTO DepartmentType(textid, name) VALUES ('hospital', 'Стационар');

