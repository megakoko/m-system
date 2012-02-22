---------------------------------------------------------------------
----------------------- Удаление таблиц -----------------------------

-- Порядок должен быть строго обратным порядку, 
-- в котором таблицы создаются.

DROP TABLE IF EXISTS DsRuleItem;
DROP TABLE IF EXISTS DsRule;
DROP TABLE IF EXISTS Operator;
DROP TABLE IF EXISTS ExaminationData;
DROP TABLE IF EXISTS UiElementEnums;
DROP TABLE IF EXISTS UiElement;
DROP TABLE IF EXISTS UiElementType;
DROP TABLE IF EXISTS Examination;
DROP TABLE IF EXISTS UserPluginAccess;
DROP TABLE IF EXISTS Plugin;
DROP TABLE IF EXISTS MUser;
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



---------------------------------------------------------------------
----------------------- Создание табьиц -----------------------------

-- Пол пациентов.
CREATE TABLE Sex (
	id 					SERIAL PRIMARY KEY,
	textid				VARCHAR (30) NOT NULL UNIQUE,
	name				VARCHAR (40) NOT NULL
);


-- Пациенты.
CREATE TABLE Patient (
	id 					SERIAL PRIMARY KEY,
	familyName 			VARCHAR (408) NOT NULL,
	name 				VARCHAR (408) NOT NULL,
	patronymic 			VARCHAR (408) NOT NULL,
	birthDay			VARCHAR (408) NOT NULL,
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
	serialNumber 		VARCHAR (408) NOT NULL,
	date 				VARCHAR (408) NOT NULL,
	givenBy 			VARCHAR (408) NOT NULL
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
	city 				VARCHAR (408) NOT NULL,
	street 				VARCHAR (408) NOT NULL,
	house 				VARCHAR (408) NOT NULL,
	apartment 			VARCHAR (408)
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
	name				VARCHAR (40) NOT NULL,
	textid				VARCHAR (40)
);


-- Таблица, связывающая отделения, персонал и должности. Определяет, 
-- кто где работает на какой должности.
CREATE TABLE DepartmentStaffPosition (
	id					SERIAL PRIMARY KEY,
	departmentId		INTEGER REFERENCES Department(id),
	staffId				INTEGER REFERENCES Staff(id),
	positionId			INTEGER REFERENCES Position(id)
);


-- Таблица с пользователями.
CREATE TABLE MUser (
	id 					SERIAL PRIMARY KEY,
	login 				VARCHAR (20) NOT NULL,
	password 			VARCHAR (32),	-- Достаточно для MD5.
	salt 				VARCHAR (20),
	is_admin 			BOOL NOT NULL DEFAULT 'false',
	-- Связанный с пользователем работник.
	attachedStaffId		INTEGER REFERENCES Staff(id)
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


-- Таблица с осмотрами пациента.
CREATE TABLE Examination (
	id					SERIAL PRIMARY KEY,
	patientId			INTEGER REFERENCES Patient(id),
	examinedByStaffId	INTEGER REFERENCES Staff(id),
	examinationDate		TIMESTAMP NOT NULL

);

-- Таблица с типами элементов интерфейса первичного осмотра.
CREATE TABLE UiElementType (
	textId				VARCHAR(40) PRIMARY KEY
);

-- Таблица с элементами интерфейса первичного осмотра.
CREATE TABLE UiElement (
	id					SERIAL PRIMARY KEY,
	textId				VARCHAR(40) NOT NULL UNIQUE,
	parentId			VARCHAR(40) REFERENCES UiElement(textId),
	availableForSexId	INTEGER REFERENCES Sex(id),			-- Пол, для которого доступен элемент.
	typeId				VARCHAR(40) REFERENCES UiElementType(textid),
	label				VARCHAR(100),
	shortLabel			VARCHAR(100)
);

-- Таблица с элементами различных списков (ComboBox, например).
CREATE TABLE UiElementEnums (
	id					INTEGER PRIMARY KEY,
	uiElementTextId		VARCHAR(40) REFERENCES UiElement(textid),
	value				VARCHAR(100) NOT NULL
);

-- Таблица, в которой хранятся введенные в форме первичного осмотра значения.
CREATE TABLE ExaminationData (
	id					SERIAL PRIMARY KEY,
	examinationId		INTEGER REFERENCES Examination(id),
	uiElementId			INTEGER REFERENCES UiElement(id),
	textValue			VARCHAR(100),
	realValue			REAL,
	enumValue			INTEGER REFERENCES UiElementEnums(id)
);


-- Операторы, которые могуть быть применены к симптомам (больше, меньше, равно и т.д.).
CREATE TABLE Operator (
	id					SERIAL PRIMARY KEY,
	textid				VARCHAR(40) UNIQUE NOT NULL,
	sign				VARCHAR(5) UNIQUE NOT NULL,
	description			VARCHAR(40) NOT NULL
);

-- Правила, по которым будет проводиться диагностика.
CREATE TABLE DsRule (
	id					SERIAL PRIMARY KEY,
	diseaseText			VARCHAR(100),
	diseaseMkbId		INTEGER REFERENCES Mkb10(id),
	diseaseProbability	REAL NOT NULL
);

-- Элемент правила DsRule.
-- Является комбинацией симптома, оператора и значения.
CREATE TABLE DsRuleItem (
	id					SERIAL PRIMARY KEY,
	ruleId				INTEGER REFERENCES DsRule(id),
	uiElementId			INTEGER REFERENCES UiElement(id),
	operatorId			INTEGER REFERENCES Operator(id),
	textValue			VARCHAR(100),
	realValue			REAL,
	realValue2			REAL,
	enumValue			INTEGER REFERENCES UiElementEnums(id),
	probabilityWithDisease		REAL NOT NULL,
	probabilityWithoutDisease	REAL NOT NULL
);


---------------------------------------------------------------------
----------------------- Данные --------------------------------------

-- Пользователь с правами админа с логином 'admin' и паролем 'pw'.
INSERT INTO MUser(login, password, salt, is_admin) VALUES
('admin', '38b311d8c359e5975c5a3f454d3f4294', 'salt', 'true');


-- При добавлении модуля надо добавлять сюда строку с его textid и названием.
INSERT INTO Plugin(textid, name) VALUES ('users', 'Пользователи системы');
INSERT INTO Plugin(textid, name) VALUES ('patients', 'Пациенты');
INSERT INTO Plugin(textid, name) VALUES ('mkb10', 'Справочная система МКБ-10');
INSERT INTO Plugin(textid, name) VALUES ('departments', 'Медицинское учреждение');
INSERT INTO Plugin(textid, name) VALUES ('test', 'Пример модуля');
INSERT INTO Plugin(textid, name) VALUES ('therapeutist', 'Терапевт');
INSERT INTO Plugin(textid, name) VALUES ('dummy_db', 'Создание тестовой БД');
INSERT INTO Plugin(textid, name) VALUES ('diagnosis_rules', 'Диагностические правила');


INSERT INTO Sex(textid, name) VALUES ('male', 'Мужчина');
INSERT INTO Sex(textid, name) VALUES ('female', 'Женщина');


INSERT INTO Position(textid, name) VALUES('therapeutist', 'Врач-терапевт');


INSERT INTO DocumentType(textid, name) VALUES ('passport', 'Паспорт');
INSERT INTO DocumentType(textid, name) VALUES ('inn', 'ИНН');
INSERT INTO DocumentType(textid, name) VALUES ('insuranceVoluntary', 'Полис добровольного страхования');
INSERT INTO DocumentType(textid, name) VALUES ('insuranceMandatory', 'Полис обязательного страхования');


INSERT INTO AddressType(textid, name) VALUES ('mailing', 'Адрес прописки');
INSERT INTO AddressType(textid, name) VALUES ('actual', 'Адрес проживания');


INSERT INTO HealthFacility(id, name) VALUES(1, 'Медицинское учреждение');


INSERT INTO DepartmentType(textid, name) VALUES ('clinic', 'Амбулаторное отделение');
INSERT INTO DepartmentType(textid, name) VALUES ('hospital', 'Стационарное отделение');


INSERT INTO Operator(textid, sign, description) VALUES ('equal', '=', 'Равняется');
INSERT INTO Operator(textid, sign, description) VALUES ('less', '<', 'Меньше');
INSERT INTO Operator(textid, sign, description) VALUES ('more', '>', 'Больше');
INSERT INTO Operator(textid, sign, description) VALUES ('between', '∈', 'В интервале');
