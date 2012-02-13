-- Типы элементов.
INSERT INTO UiElementType
SELECT 'container'
UNION SELECT 'combobox'
UNION SELECT 'lineedit'
UNION SELECT 'textedit'
UNION SELECT 'spinbox';


-- Элементы нулевого уровня и элементы первого уровня без дочерних элементов.
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 01, 'main', NULL, CAST(NULL AS "numeric"), 'container', NULL, NULL UNION
SELECT 02, 'Pre', 'main', NULL, 'textedit', 'Вводная часть', NULL UNION
SELECT 03, 'HospAim', 'main', NULL, 'combobox', 'Цель госпитализации', NULL UNION
SELECT 04, 'Complaints', 'main', NULL, 'textedit', 'Жалобы', NULL UNION
SELECT 05, 'AA_FT', 'main', NULL, 'textedit', 'Анамнез болезни', NULL UNION
SELECT 16000, 'StatusLocalis', 'main', NULL, 'textedit', 'Status localis', NULL UNION
SELECT 16010, 'Extra', 'main', NULL, 'textedit', 'Дополнительно', NULL UNION
SELECT 16020, 'EntranceDiagnosis', 'main', NULL, 'textedit', 'Ds.', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 1, 'HospAim', 'обследование' UNION
SELECT 2, 'HospAim', 'консервативное лечение' UNION
SELECT 3, 'HospAim', 'оперативное вмешательство' UNION
SELECT 4, 'HospAim', 'травма, связанная с производством' UNION
SELECT 5, 'HospAim', 'травма, не связанная с производством';


-- ************************************************************ --
-- Далее идут элементы первого уровня с их дочерними элементами --
-- ************************************************************ --


-------------------
-- Анамнез жизни --
-------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 10, 'PA', 'main', CAST(NULL AS "numeric"), 'container', 'Анамнез жизни', NULL UNION
SELECT 20, 'PA_PD', 'PA', NULL, 'textedit', 'Перенесённые заболевания', NULL UNION
SELECT 30, 'PA_TO', 'PA', NULL, 'textedit', 'Травмы и операции', NULL UNION
SELECT 40, 'PA_VH', 'PA', NULL, 'textedit', 'Вирусный гепатит', NULL UNION
SELECT 50, 'PA_Tu', 'PA', NULL, 'textedit', 'Туберкулёз', NULL UNION
SELECT 60, 'PA_VD', 'PA', NULL, 'textedit', 'Венерические заболевания', NULL UNION
SELECT 70, 'PA_PM', 'PA', NULL, 'textedit', 'Сахарный диабет', NULL UNION
SELECT 80, 'PA_EA', 'PA', NULL, 'combobox', 'Эпидемиологический анамнез', NULL UNION
SELECT 90, 'PA_AA', 'PA', NULL, 'textedit', 'Аллергологический анамнез', NULL UNION
SELECT 100, 'PA_HT', 'PA', NULL, 'textedit', 'Гемотрансфузии в анамнезе', NULL UNION
SELECT 110, 'PA_UI', 'PA', NULL, 'combobox', 'Привычные интоксикации', NULL UNION
SELECT 120, 'PA_AH', 'PA', NULL, 'textedit', 'Наследственность', NULL UNION
SELECT 130, 'PA_FS', 'PA', NULL, 'textedit', 'Семейное положение', NULL UNION
SELECT 140, 'PA_UP', 'PA', NULL, 'combobox', 'Условия проживания', NULL UNION
SELECT 150, 'PA_OGA', 'PA', NULL, 'textedit', 'Акушерско-гинекологический анамнез', NULL UNION
SELECT 160, 'PA_M', 'PA', NULL, 'textedit', 'Menses', NULL UNION
--SELECT 170, 'PA_exp', 'PA', NULL, 'combobox', 'Страховой анамнез', NULL UNION
SELECT 180, 'PAd', 'PA', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 10, 'PA_EA', 'контакты с инфекционными больными отрицает' UNION
SELECT 20, 'PA_EA', 'употребление недоброкачественных продуктов отрицает' UNION
SELECT 30, 'PA_UI', 'отрицает' UNION
SELECT 40, 'PA_UI', 'курит' UNION
SELECT 50, 'PA_UI', 'употребляет алкоголь' UNION
SELECT 60, 'PA_UI', 'употребляет наркотические вещества' UNION
SELECT 70, 'PA_UP', 'проживает в изолированной квартире со всеми удобствами' UNION
SELECT 80, 'PA_UP', 'проживает в собственном доме со всеми удобствами' UNION
SELECT 90, 'PA_UP', 'проживает в собственном доме с частичными удобствами' UNION
SELECT 100, 'PA_UP', 'проживает в собственном доме без удобств' UNION
SELECT 110, 'PA_UP', 'проживает в общежитии' UNION
SELECT 120, 'PA_UP', 'не имеет постоянного места жительства';-- UNION
--SELECT 130, 'PA_exp', 'работающий' UNION
--SELECT 140, 'PA_exp', 'неработающий' UNION
--SELECT 150, 'PA_exp', 'инвалид III группы' UNION
--SELECT 160, 'PA_exp', 'инвалид II группы' UNION
--SELECT 170, 'PA_exp', 'инвалид I группы' UNION
--SELECT 180, 'PA_exp', 'открытого больничного листа нет' UNION
--SELECT 190, 'PA_exp', 'открыт больничный лист № с' UNION
--SELECT 200, 'PA_exp', 'КЭК проводилась'


------------------
-- Общий осмотр --
------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 1010, 'USall', 'main', CAST(NULL AS "numeric"), 'container', 'Общий осмотр', NULL UNION
SELECT 1020, 'US_GC', 'USall', NULL, 'combobox', 'Общее состояние', NULL UNION
SELECT 1030, 'US_C', 'USall', NULL, 'combobox', 'Сознание', NULL UNION
SELECT 1040, 'US_PP', 'USall', NULL, 'combobox', 'Положение пациента', NULL UNION
SELECT 1050, 'US_Look', 'USall', NULL, 'combobox', 'Выражение лица', NULL UNION
SELECT 1060, 'US_Gait', 'USall', NULL, 'combobox', 'Походка', NULL UNION
SELECT 1070, 'US_BB', 'USall', NULL, 'combobox', 'Телосложение', NULL UNION
SELECT 1090, 'US_MB', 'USall', NULL, 'spinbox', 'Масса тела, кг', NULL UNION
SELECT 1100, 'US_Stature', 'USall', NULL, 'spinbox', 'Рост, см', NULL UNION
SELECT 1110, 'US_TB', 'USall', NULL, 'spinbox', 'Температура тела, °C', NULL UNION
SELECT 1120, 'US_Coverlet', 'USall', NULL, 'combobox', 'Кожные покровы', NULL UNION
SELECT 1130, 'US_MV', 'USall', NULL, 'combobox', 'Видимые слизистые', NULL UNION
SELECT 1140, 'US_SF', 'USall', NULL, 'combobox', 'Подкожно-жировая клетчатка', NULL UNION
SELECT 1150, 'US_E', 'USall', NULL, 'textedit', 'Отеки', NULL UNION
SELECT 1160, 'US_S', 'USall', NULL, 'container', 'Придатки кожи', NULL UNION
SELECT 1180, 'US_SHt', 'US_S', NULL, 'textedit', 'Волосы', NULL UNION
SELECT 1190, 'US_SNt', 'US_S', NULL, 'textedit', 'Ногти', NULL UNION
SELECT 1200, 'US_SMt', 'US_S', NULL, 'textedit', 'Молочные железы', NULL UNION
SELECT 1210, 'US_Gd', 'USall', NULL, 'combobox', 'Лимфатические узлы', NULL UNION
SELECT 1220, 'US_G', 'USall', NULL, 'container', 'Лимфатические узлы', NULL UNION
SELECT 1230, 'US_Gsm', 'US_G', NULL, 'combobox', 'Подчелюстные лимфатические узлы', 'подчелюстные' UNION
SELECT 1240, 'US_Gce', 'US_G', NULL, 'combobox', 'Шейные лимфатические узлы', 'шейные' UNION
SELECT 1250, 'US_Gsc', 'US_G', NULL, 'combobox', 'Надключичные лимфатические узлы', 'надключичные' UNION
SELECT 1260, 'US_Gbc', 'US_G', NULL, 'combobox', 'Подключичные лимфатические узлы', 'подключичные' UNION
SELECT 1270, 'US_Gsa', 'US_G', NULL, 'combobox', 'Подмышечные лимфатические узлы', 'подмышечные' UNION
SELECT 1280, 'US_Glo', 'US_G', NULL, 'combobox', 'Локтевые лимфатические узлы', 'локтевые' UNION
SELECT 1290, 'US_Gpa', 'US_G', NULL, 'combobox', 'Паховые лимфатические узлы', 'паховые' UNION
SELECT 1300, 'US_Gbe', 'US_G', NULL, 'combobox', 'Бедренные лимфатические узлы', 'бедренные' UNION
SELECT 1310, 'USd', 'USall', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 1010, 'US_GC', 'удовлетворительное' UNION
SELECT 1020, 'US_GC', 'средней тяжести' UNION
SELECT 1030, 'US_GC', 'тяжёлое' UNION
SELECT 1040, 'US_GC', 'крайне тяжёлое' UNION
SELECT 1050, 'US_GC', 'агональное' UNION
SELECT 1060, 'US_C', 'ясное' UNION
SELECT 1070, 'US_C', 'угнетенное' UNION
SELECT 1080, 'US_C', 'пациент в состоянии ступора' UNION
SELECT 1090, 'US_C', 'пациент в состоянии сопора' UNION
SELECT 1100, 'US_C', 'пациент в состоянии оглушенности' UNION
SELECT 1110, 'US_C', 'пациент в состоянии загруженности' UNION
SELECT 1120, 'US_C', 'пациент в состоянии психомоторного возбуждения' UNION
SELECT 1130, 'US_C', 'кома I' UNION
SELECT 1140, 'US_C', 'кома II' UNION
SELECT 1150, 'US_C', 'кома III' UNION
SELECT 1160, 'US_C', 'отсутствует' UNION
SELECT 1170, 'US_PP', 'активное' UNION
SELECT 1180, 'US_PP', 'пассивное' UNION
SELECT 1190, 'US_PP', 'вынужденное' UNION
SELECT 1200, 'US_Look', 'возбужденное' UNION
SELECT 1210, 'US_Look', 'спокойное' UNION
SELECT 1220, 'US_Look', 'безразличное' UNION
SELECT 1230, 'US_Look', 'страдальческое' UNION
SELECT 1240, 'US_Look', 'маскообразное' UNION
SELECT 1250, 'US_Gait', 'передвигается свободно' UNION
SELECT 1260, 'US_Gait', 'при ходьбе хромает' UNION
SELECT 1270, 'US_Gait', 'при ходьбе пользуется костылями' UNION
SELECT 1280, 'US_Gait', 'при ходьбе пользуется палкой' UNION
SELECT 1290, 'US_BB', 'правильное' UNION
SELECT 1300, 'US_BB', 'неправильное' UNION
SELECT 1340, 'US_Coverlet', 'физиологической окраски' UNION
SELECT 1350, 'US_Coverlet', 'гиперемированные' UNION
SELECT 1360, 'US_Coverlet', 'бледноватые' UNION
SELECT 1370, 'US_Coverlet', 'бледные' UNION
SELECT 1380, 'US_Coverlet', 'субиктеричные' UNION
SELECT 1390, 'US_Coverlet', 'иктеричные' UNION
SELECT 1400, 'US_Coverlet', 'цианотичные' UNION
SELECT 1410, 'US_Coverlet', 'землистого оттенка' UNION
SELECT 1420, 'US_Coverlet', 'сухие' UNION
SELECT 1430, 'US_Coverlet', 'влажные' UNION
SELECT 1440, 'US_Coverlet', 'эластичные' UNION
SELECT 1450, 'US_Coverlet', 'без сыпи' UNION
SELECT 1460, 'US_Coverlet', 'тургор снижен' UNION
SELECT 1470, 'US_MV', 'физиологической окраски' UNION
SELECT 1480, 'US_MV', 'гиперемированные' UNION
SELECT 1490, 'US_MV', 'бледноватые' UNION
SELECT 1500, 'US_MV', 'бледные' UNION
SELECT 1510, 'US_MV', 'субиктеричные' UNION
SELECT 1520, 'US_MV', 'иктеричные' UNION
SELECT 1530, 'US_MV', 'цианотичные' UNION
SELECT 1540, 'US_MV', 'землистого оттенка' UNION
SELECT 1550, 'US_MV', 'сухие' UNION
SELECT 1560, 'US_MV', 'влажные' UNION
SELECT 1570, 'US_MV', 'эластичные' UNION
SELECT 1580, 'US_SF', 'развита слабо' UNION
SELECT 1590, 'US_SF', 'развита умеренно' UNION
SELECT 1600, 'US_SF', 'развита чрезмерно' UNION
SELECT 1620, 'US_Gd', 'без особенностей' UNION
SELECT 1630, 'US_Gd', 'не увеличены' UNION
SELECT 1640, 'US_Gd', 'подвижные' UNION
SELECT 1650, 'US_Gd', 'не спаяны с кожей' UNION
SELECT 1660, 'US_Gd', 'безболезненные' UNION
SELECT 1670, 'US_Gd', 'увеличены' UNION
SELECT 1680, 'US_Gd', 'неподвижные' UNION
SELECT 1690, 'US_Gd', 'спаяны с кожей' UNION
SELECT 1700, 'US_Gd', 'болезненные' UNION
SELECT 1710, 'US_Gd', 'кожа над л/у гиперемирована' UNION
SELECT 1720, 'US_Gd', 'наблюдается локальная гипертермия в области узлов' UNION
SELECT 1730, 'US_Gsm', 'не изменены' UNION
SELECT 1740, 'US_Gsm', 'не увеличены' UNION
SELECT 1750, 'US_Gsm', 'подвижные' UNION
SELECT 1760, 'US_Gsm', 'не спаяны с кожей' UNION
SELECT 1770, 'US_Gsm', 'безболезненные' UNION
SELECT 1780, 'US_Gsm', 'увеличены' UNION
SELECT 1790, 'US_Gsm', 'неподвижные' UNION
SELECT 1800, 'US_Gsm', 'спаяны с кожей' UNION
SELECT 1810, 'US_Gsm', 'болезненные' UNION
SELECT 1820, 'US_Gsm', 'кожа над л/у гиперемирована' UNION
SELECT 1830, 'US_Gsm', 'наблюдается локальная гипертермия в области узлов' UNION
SELECT 1840, 'US_Gce', 'не изменены' UNION
SELECT 1850, 'US_Gce', 'не увеличены' UNION
SELECT 1860, 'US_Gce', 'подвижные' UNION
SELECT 1870, 'US_Gce', 'не спаяны с кожей' UNION
SELECT 1880, 'US_Gce', 'безболезненные' UNION
SELECT 1890, 'US_Gce', 'увеличены' UNION
SELECT 1900, 'US_Gce', 'неподвижные' UNION
SELECT 1910, 'US_Gce', 'спаяны с кожей' UNION
SELECT 1920, 'US_Gce', 'болезненные' UNION
SELECT 1930, 'US_Gce', 'кожа над л/у гиперемирована' UNION
SELECT 1940, 'US_Gce', 'наблюдается локальная гипертермия в области узлов' UNION
SELECT 1950, 'US_Gsc', 'не изменены' UNION
SELECT 1960, 'US_Gsc', 'не увеличены' UNION
SELECT 1970, 'US_Gsc', 'подвижные' UNION
SELECT 1980, 'US_Gsc', 'не спаяны с кожей' UNION
SELECT 1990, 'US_Gsc', 'безболезненные' UNION
SELECT 2000, 'US_Gsc', 'увеличены' UNION
SELECT 2010, 'US_Gsc', 'неподвижные' UNION
SELECT 2020, 'US_Gsc', 'спаяны с кожей' UNION
SELECT 2030, 'US_Gsc', 'болезненные' UNION
SELECT 2040, 'US_Gsc', 'кожа над л/у гиперемирована' UNION
SELECT 2050, 'US_Gsc', 'наблюдается локальная гипертермия в области узлов' UNION
SELECT 2060, 'US_Gbc', 'не изменены' UNION
SELECT 2070, 'US_Gbc', 'не увеличены' UNION
SELECT 2080, 'US_Gbc', 'подвижные' UNION
SELECT 2090, 'US_Gbc', 'не спаяны с кожей' UNION
SELECT 2100, 'US_Gbc', 'безболезненные' UNION
SELECT 2110, 'US_Gbc', 'увеличены' UNION
SELECT 2120, 'US_Gbc', 'неподвижные' UNION
SELECT 2130, 'US_Gbc', 'спаяны с кожей' UNION
SELECT 2140, 'US_Gbc', 'болезненные' UNION
SELECT 2150, 'US_Gbc', 'кожа над л/у гиперемирована' UNION
SELECT 2160, 'US_Gbc', 'наблюдается локальная гипертермия в области узлов' UNION
SELECT 2170, 'US_Gsa', 'не изменены' UNION
SELECT 2180, 'US_Gsa', 'не увеличены' UNION
SELECT 2190, 'US_Gsa', 'подвижные' UNION
SELECT 2200, 'US_Gsa', 'не спаяны с кожей' UNION
SELECT 2210, 'US_Gsa', 'безболезненные' UNION
SELECT 2220, 'US_Gsa', 'увеличены' UNION
SELECT 2230, 'US_Gsa', 'неподвижные' UNION
SELECT 2240, 'US_Gsa', 'спаяны с кожей' UNION
SELECT 2250, 'US_Gsa', 'болезненные' UNION
SELECT 2260, 'US_Gsa', 'кожа над л/у гиперемирована' UNION
SELECT 2270, 'US_Gsa', 'наблюдается локальная гипертермия в области узлов' UNION
SELECT 2280, 'US_Glo', 'не изменены' UNION
SELECT 2290, 'US_Glo', 'не увеличены' UNION
SELECT 2300, 'US_Glo', 'подвижные' UNION
SELECT 2310, 'US_Glo', 'не спаяны с кожей' UNION
SELECT 2320, 'US_Glo', 'безболезненные' UNION
SELECT 2330, 'US_Glo', 'увеличены' UNION
SELECT 2340, 'US_Glo', 'неподвижные' UNION
SELECT 2350, 'US_Glo', 'спаяны с кожей' UNION
SELECT 2360, 'US_Glo', 'болезненные' UNION
SELECT 2370, 'US_Glo', 'кожа над л/у гиперемирована' UNION
SELECT 2380, 'US_Glo', 'наблюдается локальная гипертермия в области узлов' UNION
SELECT 2390, 'US_Gpa', 'не изменены' UNION
SELECT 2400, 'US_Gpa', 'не увеличены' UNION
SELECT 2410, 'US_Gpa', 'подвижные' UNION
SELECT 2420, 'US_Gpa', 'не спаяны с кожей' UNION
SELECT 2430, 'US_Gpa', 'безболезненные' UNION
SELECT 2440, 'US_Gpa', 'увеличены' UNION
SELECT 2450, 'US_Gpa', 'неподвижные' UNION
SELECT 2460, 'US_Gpa', 'спаяны с кожей' UNION
SELECT 2470, 'US_Gpa', 'болезненные' UNION
SELECT 2480, 'US_Gpa', 'кожа над л/у гиперемирована' UNION
SELECT 2490, 'US_Gpa', 'наблюдается локальная гипертермия в области узлов' UNION
SELECT 2500, 'US_Gbe', 'не изменены' UNION
SELECT 2510, 'US_Gbe', 'не увеличены' UNION
SELECT 2520, 'US_Gbe', 'подвижные' UNION
SELECT 2530, 'US_Gbe', 'не спаяны с кожей' UNION
SELECT 2540, 'US_Gbe', 'безболезненные' UNION
SELECT 2550, 'US_Gbe', 'увеличены' UNION
SELECT 2560, 'US_Gbe', 'неподвижные' UNION
SELECT 2570, 'US_Gbe', 'спаяны с кожей' UNION
SELECT 2580, 'US_Gbe', 'болезненные' UNION
SELECT 2590, 'US_Gbe', 'кожа над л/у гиперемирована' UNION
SELECT 2600, 'US_Gbe', 'наблюдается локальная гипертермия в области узлов';


---------------------------------
-- Опорно-двигательный аппарат --
---------------------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 3010, 'MSSall', 'main', CAST(NULL AS "numeric"), 'container', 'Опорно-двигательный аппарат', NULL UNION
SELECT 3020, 'MSSS', 'MSSall', NULL, 'textedit', 'Костные структуры', NULL UNION
SELECT 3030, 'MSSM', 'MSSall', NULL, 'container', 'Мышечная система', NULL UNION
SELECT 3040, 'NS_dva', 'MSSM', NULL, 'textedit', 'Атрофия мышц', NULL UNION
SELECT 3050, 'NS_dvg', 'MSSM', NULL, 'textedit', 'Истинная гипертрофия мышц', NULL UNION
SELECT 3060, 'NS_dvp', 'MSSM', NULL, 'textedit', 'Псевдогипертрофии', NULL UNION
SELECT 3070, 'NS_dvf', 'MSSM', NULL, 'textedit', 'Фибриллярные и фасцикулярные', NULL UNION
SELECT 3080, 'NS_dvr', 'MSSM', NULL, 'textedit', 'Ретракции и контрактуры', NULL UNION
SELECT 3090, 'NS_dvo', 'MSSM', NULL, 'textedit', 'Объем движений в конечностях', NULL UNION
SELECT 3100, 'NS_dvs', 'MSSM', NULL, 'textedit', 'Сила мышц дистальных и', NULL UNION
SELECT 3110, 'NS_dvt', 'MSSM', NULL, 'textedit', 'Тонус мышц', NULL UNION
SELECT 3120, 'MSSJ', 'MSSall', NULL, 'container', 'Суставы', NULL UNION
SELECT 3130, 'MSSJ_1', 'MSSJ', NULL, 'textedit', 'Конфигурация суставов', 'Конфигурация' UNION
SELECT 3140, 'MSSJ_2', 'MSSJ', NULL, 'textedit', 'Болезненность суставов при пальпации', 'Болезненность при пальпации' UNION
SELECT 3150, 'MSSJ_3', 'MSSJ', NULL, 'textedit', 'Активные движения суставов', 'Активные движения' UNION
SELECT 3160, 'MSSJ_4', 'MSSJ', NULL, 'textedit', 'Пассивные движения суставов', 'Пассивные движения' UNION
SELECT 3170, 'MSSd', 'MSSall', NULL, 'textedit', 'Дополнительно', NULL;


---------
-- Шея --
---------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 4010, 'Neckall', 'main', CAST(NULL AS "numeric"), 'container', 'Шея', NULL UNION
SELECT 4020, 'NeckF', 'Neckall', NULL, 'textedit', 'Форма шеи', NULL UNION
SELECT 4030, 'NeckV', 'Neckall', NULL, 'combobox', 'Голос', NULL UNION
SELECT 4040, 'NeckTG', 'Neckall', NULL, 'container', 'Щитовидная железа', NULL UNION
SELECT 4050, 'NeckTGS', 'NeckTG', NULL, 'combobox', 'Размеры щитовидной железы', 'Размеры' UNION
SELECT 4060, 'NeckTGM', 'NeckTG', NULL, 'combobox', 'Подвижность щитовидной железы', 'Подвижность' UNION
SELECT 4070, 'NeckTGP', 'NeckTG', NULL, 'combobox', 'Болезненность щитовидной железы при пальпации', 'Болезненность при пальпации' UNION
SELECT 4080, 'NeckTGCS', 'NeckTG', NULL, 'combobox', 'Консистенция щитовидной железы', 'Консистенция' UNION
SELECT 4090, 'NeckTGSU', 'NeckTG', NULL, 'combobox', 'Поверхность щитовидной железы', 'Поверхность' UNION
SELECT 4100, 'NeckD', 'Neckall', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 4010, 'NeckV', 'чёткий' UNION
SELECT 4020, 'NeckV', 'осиплый' UNION
SELECT 4030, 'NeckV', 'хриплый' UNION
SELECT 4040, 'NeckV', 'наблюдается афазия' UNION
SELECT 4050, 'NeckTG', 'не увеличена' UNION
SELECT 4060, 'NeckTG', 'увеличение I степени' UNION
SELECT 4070, 'NeckTG', 'увеличение II степени' UNION
SELECT 4080, 'NeckTG', 'увеличение III степени' UNION
SELECT 4090, 'NeckTG', 'увеличение IV степени' UNION
SELECT 4100, 'NeckTGS', 'не увеличена' UNION
SELECT 4110, 'NeckTGS', 'увеличение I степени' UNION
SELECT 4120, 'NeckTGS', 'увеличение II степени' UNION
SELECT 4130, 'NeckTGS', 'увеличение III степени' UNION
SELECT 4140, 'NeckTGS', 'увеличение IV степени' UNION
SELECT 4150, 'NeckTGM', 'при глотании смещается' UNION
SELECT 4160, 'NeckTGM', 'при глотании не смещается' UNION
SELECT 4170, 'NeckTGP', 'при пальпации безболезненная' UNION
SELECT 4180, 'NeckTGP', 'при пальпации болезненная' UNION
SELECT 4190, 'NeckTGCS', 'мягкая' UNION
SELECT 4200, 'NeckTGCS', 'плотная' UNION
SELECT 4210, 'NeckTGSU', 'поверхность гладкая' UNION
SELECT 4220, 'NeckTGSU', 'поверхность бугристая';


-------------------
-- Органы зрения --
-------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 5010, 'OVall', 'main', CAST(NULL AS "numeric"), 'container', 'Органы зрения', NULL UNION
SELECT 5020, 'NS_cnzo', 'OVall', NULL, 'textedit', 'Острота зрения', NULL UNION
SELECT 5030, 'NS_cnzc', 'OVall', NULL, 'textedit', 'Цветоощущение', NULL UNION
SELECT 5040, 'NS_cnzp', 'OVall', NULL, 'textedit', 'Поля зрения', NULL UNION
SELECT 5050, 'NS_cngd', 'OVall', NULL, 'textedit', 'Диплопия', NULL UNION
SELECT 5060, 'US_OVemot', 'OVall', NULL, 'textedit', 'Движения глазных яблок', NULL UNION
SELECT 5070, 'US_OVMio', 'OVall', NULL, 'combobox', 'Миоз', NULL UNION
SELECT 5080, 'US_OVMyd', 'OVall', NULL, 'combobox', 'Мидриаз', NULL UNION
SELECT 5090, 'US_OVEno', 'OVall', NULL, 'combobox', 'Энофтальм', NULL UNION
SELECT 5100, 'US_OVExo', 'OVall', NULL, 'combobox', 'Экзофтальм', NULL UNION
SELECT 5110, 'US_OVCS', 'OVall', NULL, 'combobox', 'Цвет склер', NULL UNION
SELECT 5120, 'US_OVd', 'OVall', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 5010, 'US_OVMio', 'правосторонний' UNION
SELECT 5020, 'US_OVMio', 'левосторонний' UNION
SELECT 5030, 'US_OVMio', 'двусторонний' UNION
SELECT 5040, 'US_OVMyd', 'правосторонний' UNION
SELECT 5050, 'US_OVMyd', 'левосторонний' UNION
SELECT 5060, 'US_OVMyd', 'двусторонний' UNION
SELECT 5070, 'US_OVEno', 'правосторонний' UNION
SELECT 5080, 'US_OVEno', 'левосторонний' UNION
SELECT 5090, 'US_OVEno', 'двусторонний' UNION
SELECT 5100, 'US_OVExo', 'правосторонний' UNION
SELECT 5110, 'US_OVExo', 'левосторонний' UNION
SELECT 5120, 'US_OVExo', 'двусторонний' UNION
SELECT 5130, 'US_OVExo', 'отсутствует' UNION
SELECT 5140, 'US_OVCS', 'обычный' UNION
SELECT 5150, 'US_OVCS', 'субиктеричный' UNION
SELECT 5160, 'US_OVCS', 'иктеричный' UNION
SELECT 5170, 'US_OVCS', 'гиперемия склер';


--------------------
-- Органы дыхания --
--------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 6010, 'RAall', 'main', CAST(NULL AS "numeric"), 'container', 'Органы дыхания', NULL UNION
SELECT 6020, 'US_RAFT', 'RAall', NULL, 'combobox', 'Форма грудной клетки', NULL UNION
SELECT 6030, 'US_RAFRM', 'RAall', NULL, 'textedit', 'Частота дыхательных движений', NULL UNION
SELECT 6040, 'US_RACB', 'RAall', NULL, 'combobox', 'Одышка', NULL UNION
SELECT 6050, 'US_RAPR', 'RAall', NULL, 'combobox', 'Патологическое дыхание', NULL UNION
SELECT 6060, 'US_RAP', 'RAall', NULL, 'textedit', 'Пальпация грудной клетки', NULL UNION
SELECT 6070, 'US_RAPS', 'RAall', NULL, 'textedit', 'Перкуторный звук', NULL UNION
SELECT 6080, 'US_RAA', 'RAall', NULL, 'combobox', 'Аускультативно', NULL UNION
SELECT 6090, 'US_RAW', 'RAall', NULL, 'textedit', 'Хрипы', NULL UNION
SELECT 6100, 'US_RAPFR', 'RAall', NULL, 'combobox', 'Шум трения плевры', NULL UNION
SELECT 6110, 'US_RAVT', 'RAall', NULL, 'combobox', 'Голосовое дрожание', NULL UNION
SELECT 6120, 'US_RAd', 'RAall', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 6010, 'US_RAFT', 'нормостеническая' UNION
SELECT 6020, 'US_RAFT', 'астеническая' UNION
SELECT 6030, 'US_RAFT', 'гиперстеническая' UNION
SELECT 6040, 'US_RAFT', 'бочкообразная' UNION
SELECT 6050, 'US_RAFT', 'паралитическая' UNION
SELECT 6060, 'US_RAFT', 'рахитическая' UNION
SELECT 6070, 'US_RAFT', 'коническая' UNION
SELECT 6080, 'US_RACB', 'отсутствует' UNION
SELECT 6090, 'US_RACB', 'экспираторного характера' UNION
SELECT 6100, 'US_RACB', 'инспираторного характера' UNION
SELECT 6110, 'US_RACB', 'смешанного характера' UNION
SELECT 6120, 'US_RAPR', 'Чейн-Стокса' UNION
SELECT 6130, 'US_RAPR', 'Кусмауля' UNION
SELECT 6140, 'US_RAPR', 'Биота' UNION
SELECT 6150, 'US_RAA', 'дыхание везикулярное, проводится по всем полям' UNION
SELECT 6160, 'US_RAA', 'дыхание бронхиальное' UNION
SELECT 6170, 'US_RAA', 'дыхание жёсткое' UNION
SELECT 6180, 'US_RAA', 'дыхание амфорическое' UNION
SELECT 6190, 'US_RAPFR', 'не выслушивается' UNION
SELECT 6200, 'US_RAPFR', 'выслушивается справа' UNION
SELECT 6210, 'US_RAPFR', 'выслушивается слева' UNION
SELECT 6220, 'US_RAPFR', 'выслушивается с обеих сторон' UNION
SELECT 6230, 'US_RAVT', 'без особенностей' UNION
SELECT 6240, 'US_RAVT', 'усилено справа' UNION
SELECT 6250, 'US_RAVT', 'ослаблено справа' UNION
SELECT 6260, 'US_RAVT', 'усилено слева' UNION
SELECT 6270, 'US_RAVT', 'ослаблено слева' UNION
SELECT 6280, 'US_RAVT', 'усилено с обех сторон' UNION
SELECT 6290, 'US_RAVT', 'ослаблено с обех сторон';


---------------------------------
-- Сердечно-сосудистая система --
---------------------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 7010, 'CVSall', 'main', CAST(NULL AS "numeric"), 'container', 'Сердечно-сосудистая система', NULL UNION
SELECT 7020, 'CVSPH', 'CVSall', NULL, 'combobox', 'Перкуссия сердца', NULL UNION
SELECT 7030, 'CVSAH', 'CVSall', NULL, 'combobox', 'Аускультация сердца', NULL UNION
SELECT 7040, 'CVSHB', 'CVSall', NULL, 'combobox', 'Сердечные сокращения', NULL UNION
SELECT 7045, 'CSSHB_', 'CVSall', NULL, 'textedit', 'Частота сердечных сокращений', NULL UNION
SELECT 7050, 'CVSP', 'CVSall', NULL, 'combobox', 'Пульс', NULL UNION
SELECT 7055, 'CVSP_', 'CVSall', NULL, 'textedit', 'Частота пульса', NULL UNION
SELECT 7060, 'CVSJS', 'CVSall', NULL, 'textedit', 'Дефицит пульса', NULL UNION
SELECT 7070, 'CVSAP', 'CVSall', NULL, 'container', 'Артериальное давление', NULL UNION
SELECT 7080, 'CVSAPRTt', 'CVSAP', NULL, 'textedit', 'Артериальное давление на правой верхней конечности', 'на правой верхней конечности' UNION
SELECT 7090, 'CVSAPLTt', 'CVSAP', NULL, 'textedit', 'Артериальное давление на левой верхней конечности', 'на левой верхней конечности' UNION
SELECT 7100, 'CVSAPRBt', 'CVSAP', NULL, 'textedit', 'Артериальное давление на правой нижней конечности', 'на правой нижней конечности' UNION
SELECT 7110, 'CVSAPLBt', 'CVSAP', NULL, 'textedit', 'Артериальное давление на левой нижней конечности', 'на левой нижней конечности' UNION
SELECT 7120, 'CVSAT', 'CVSall', NULL, 'combobox', 'Верхушечный толчок', NULL UNION
SELECT 7130, 'CVSHT', 'CVSall', NULL, 'textedit', 'Сердечный толчок', NULL UNION
SELECT 7140, 'CVSPM', 'CVSall', NULL, 'combobox', 'Шум трения перикарда', NULL UNION
SELECT 7150, 'CVSCCAR', 'CVSall', NULL, 'container', 'Правая общая сонная артерия', NULL UNION
SELECT 7160, 'CVSCCARP', 'CVSCCAR', NULL, 'combobox', 'Пульсация правой общей сонной артерии', 'пульсация' UNION
SELECT 7170, 'CVSCCARN', 'CVSCCAR', NULL, 'combobox', 'Шумы правой общей сонной артерии', 'шумы' UNION
SELECT 7180, 'CVSCCARo', 'CVSCCAR', NULL, 'textedit', 'Правая общая сонная артерия (дополнительно)', 'дополнительно' UNION
SELECT 7190, 'CVSCCAL', 'CVSall', NULL, 'container', 'Левая общая сонная артерия', NULL UNION
SELECT 7200, 'CVSCCALP', 'CVSCCAL', NULL, 'combobox', 'Пульсация левой общей сонной артерии', 'пульсация' UNION
SELECT 7210, 'CVSCCALN', 'CVSCCAL', NULL, 'combobox', 'Шумы левой общей сонной артерии', 'шумы' UNION
SELECT 7220, 'CVSCCALo', 'CVSCCAL', NULL, 'textedit', 'Левая общая сонная артерия (дополнительно)', 'дополнительно' UNION
SELECT 7230, 'CVSCAR', 'CVSall', NULL, 'container', 'Правая подключичная артерия', NULL UNION
SELECT 7240, 'CVSCARN', 'CVSCAR', NULL, 'combobox', 'Шумы правой подключичной артерии', 'шумы' UNION
SELECT 7250, 'CVSCARo', 'CVSCAR', NULL, 'textedit', 'Правая подключичная артерия (дополнительно)', 'дополнительно' UNION
SELECT 7260, 'CVSCAL', 'CVSall', NULL, 'container', 'Левая подключичная артерия', NULL UNION
SELECT 7270, 'CVSCALN', 'CVSCAL', NULL, 'combobox', 'Шумы левой подключичной артерии', 'шумы' UNION
SELECT 7280, 'CVSCALo', 'CVSCAL', NULL, 'textedit', 'Левая подключичная артерия (дополнительно)', 'дополнительно' UNION
SELECT 7290, 'CVSBAR', 'CVSall', NULL, 'container', 'Правая плечевая артерия', NULL UNION
SELECT 7300, 'CVSBARP', 'CVSBAR', NULL, 'combobox', 'Пульсация правой плечевой артерии', 'пульсация' UNION
SELECT 7310, 'CVSBARN', 'CVSBAR', NULL, 'combobox', 'Шумы правой плечевой артерии', 'шумы' UNION
SELECT 7320, 'CVSBARo', 'CVSBAR', NULL, 'textedit', 'Правая плечевая артерия (дополнительно)', 'дополнительно' UNION
SELECT 7330, 'CVSBAL', 'CVSall', NULL, 'container', 'Левая плечевая артерия', NULL UNION
SELECT 7340, 'CVSBALP', 'CVSBAL', NULL, 'combobox', 'Пульсация левой плечевой артерии', 'пульсация' UNION
SELECT 7350, 'CVSBALN', 'CVSBAL', NULL, 'combobox', 'Шумы левой плечевой артерии', 'шумы' UNION
SELECT 7360, 'CVSBALo', 'CVSBAL', NULL, 'textedit', 'Левая плечевая артерия (дополнительно)', 'дополнительно' UNION
SELECT 7370, 'CVSRAR', 'CVSall', NULL, 'container', 'Правая лучевая артерия', NULL UNION
SELECT 7380, 'CVSRARP', 'CVSRAR', NULL, 'combobox', 'Пульсация правой лучевой артерии', 'пульсация' UNION
SELECT 7390, 'CVSRARo', 'CVSRAR', NULL, 'textedit', 'Правая лучевая артерия (дополнительно)', 'дополнительно' UNION
SELECT 7400, 'CVSRAL', 'CVSall', NULL, 'container', 'Левая лучевая артерия', NULL UNION
SELECT 7410, 'CVSRALP', 'CVSRAL', NULL, 'combobox', 'Пульсация левой лучевой артерии', 'пульсация' UNION
SELECT 7420, 'CVSRALo', 'CVSRAL', NULL, 'textedit', 'Левая лучевая артерия (дополнительно)', 'дополнительно' UNION
SELECT 7430, 'CVSAA', 'CVSall', NULL, 'container', 'Брюшная аорта', NULL UNION
SELECT 7440, 'CVSAAP', 'CVSAA', NULL, 'combobox', 'Пульсация брюшной аорты', 'пульсация' UNION
SELECT 7450, 'CVSAAN', 'CVSAA', NULL, 'combobox', 'Шумы брюшной аорты', 'шумы' UNION
SELECT 7460, 'CVSAAo', 'CVSAA', NULL, 'textedit', 'Брюшная аорта (дополнительно)', 'дополнительно' UNION
SELECT 7470, 'CVSFAR', 'CVSall', NULL, 'container', 'Правая общая бедренная артерия', NULL UNION
SELECT 7480, 'CVSFARP', 'CVSFAR', NULL, 'combobox', 'Пульсация правой общей бедренной артерии', 'пульсация' UNION
SELECT 7490, 'CVSFARN', 'CVSFAR', NULL, 'combobox', 'Шумы правой общей бедренной артерии', 'шумы' UNION
SELECT 7500, 'CVSFARo', 'CVSFAR', NULL, 'textedit', 'Правая общая бедренная артерия (дополнительно)', 'дополнительно' UNION
SELECT 7510, 'CVSFAL', 'CVSall', NULL, 'container', 'Левая общая бедренная артерия', NULL UNION
SELECT 7520, 'CVSFALP', 'CVSFAL', NULL, 'combobox', 'Пульсация левой общей бедренной артерии', 'пульсация' UNION
SELECT 7530, 'CVSFALN', 'CVSFAL', NULL, 'combobox', 'Шумы левой общей бедренной артерии', 'шумы' UNION
SELECT 7540, 'CVSFALo', 'CVSFAL', NULL, 'textedit', 'Левая общая бедренная артерия (дополнительно)', 'дополнительно' UNION
SELECT 7550, 'CVSPAR', 'CVSall', NULL, 'container', 'Правая подколенная артерия', NULL UNION
SELECT 7560, 'CVSPARP', 'CVSPAR', NULL, 'combobox', 'Пульсация правой подколенной артерии', 'пульсация' UNION
SELECT 7570, 'CVSPARN', 'CVSPAR', NULL, 'combobox', 'Шумы правой подколенной артерии', 'шумы' UNION
SELECT 7580, 'CVSPARo', 'CVSPAR', NULL, 'textedit', 'Правая подколенная артерия (дополнительно)', 'дополнительно' UNION
SELECT 7590, 'CVSPAL', 'CVSall', NULL, 'container', 'Левая подколенная артерия', NULL UNION
SELECT 7600, 'CVSPALP', 'CVSPAL', NULL, 'combobox', 'Пульсация левой подколенной артерии', 'пульсация' UNION
SELECT 7610, 'CVSPALN', 'CVSPAL', NULL, 'combobox', 'Шумы левой подколенной артерии', 'шумы' UNION
SELECT 7620, 'CVSPALo', 'CVSPAL', NULL, 'textedit', 'Левая подколенная артерия (дополнительно)', 'дополнительно' UNION
SELECT 7630, 'CVSRFAR', 'CVSall', NULL, 'container', 'Правая артерия тыла стопы', NULL UNION
SELECT 7640, 'CVSRFARP', 'CVSRFAR', NULL, 'combobox', 'Пульсация правой артерии тыла стопы', 'пульсация' UNION
SELECT 7650, 'CVSRFARo', 'CVSRFAR', NULL, 'textedit', 'Правая артерия тыла стопы (дополнительно)', 'дополнительно' UNION
SELECT 7660, 'CVSRFAL', 'CVSall', NULL, 'container', 'Левая артерия тыла стопы', NULL UNION
SELECT 7670, 'CVSRFALP', 'CVSRFAL', NULL, 'combobox', 'Пульсация левой артерии тыла стопы', 'пульсация' UNION
SELECT 7680, 'CVSRFALo', 'CVSRFAL', NULL, 'textedit', 'Левая артерия тыла стопы (дополнительно)', 'дополнительно' UNION
SELECT 7690, 'CVSPTAR', 'CVSall', NULL, 'container', 'Правая задняя большеберцовая артерия', NULL UNION
SELECT 7700, 'CVSPTARP', 'CVSPTAR', NULL, 'combobox', 'Пульсация правой задней большеберцовой артерии', 'пульсация' UNION
SELECT 7710, 'CVSPTARo', 'CVSPTAR', NULL, 'textedit', 'Правая задняя большеберцовая артерия (дополнительно)', 'дополнительно' UNION
SELECT 7720, 'CVSPTAL', 'CVSall', NULL, 'container', 'Левая задняя большеберцовая артерия', NULL UNION
SELECT 7730, 'CVSPTALP', 'CVSPTAL', NULL, 'combobox', 'Пульсация левой задней большеберцовой артерии', 'пульсация' UNION
SELECT 7740, 'CVSPTALo', 'CVSPTAL', NULL, 'textedit', 'Левая задняя большеберцовая артерия (дополнительно)', 'дополнительно' UNION
SELECT 7750, 'CVSSVT', 'CVSall', NULL, 'textedit', 'Подкожные вены верхних конечностей', NULL UNION
SELECT 7760, 'CVSSVB', 'CVSall', NULL, 'textedit', 'Подкожные вены нижних конечностей', NULL UNION
SELECT 7770, 'CVSCE', 'CVSall', NULL, 'container', 'Окружность конечностей', NULL UNION
SELECT 7780, 'CVSCEd1', 'CVSCE', NULL, 'textedit', 'Окружность правой верхней конечности в средней трети плеча', 'правой верхней в средней трети плеча' UNION
SELECT 7790, 'CVSCEd2', 'CVSCE', NULL, 'textedit', 'Окружность левой верхней конечности в средней трети плеча', 'левой верхней в средней трети плеча' UNION
SELECT 7800, 'CVSCEd3', 'CVSCE', NULL, 'textedit', 'Окружность правой верхней конечности в средней трети предплечья', 'правой верхней в средней трети предплечья' UNION
SELECT 7810, 'CVSCEd4', 'CVSCE', NULL, 'textedit', 'Окружность левой верхней конечности в средней трети предплечья', 'левой верхней в средней трети предплечья' UNION
SELECT 7820, 'CVSCEd5', 'CVSCE', NULL, 'textedit', 'Окружность правой нижней конечности в средней трети бедра', 'правой нижней в средней трети бедра' UNION
SELECT 7830, 'CVSCEd6', 'CVSCE', NULL, 'textedit', 'Окружность левой нижней конечности в средней трети бедра', 'левой нижней в средней трети бедра' UNION
SELECT 7840, 'CVSCEd7', 'CVSCE', NULL, 'textedit', 'Окружность правой нижней конечности в средней трети голени', 'правой нижней в средней трети голени' UNION
SELECT 7850, 'CVSCEd8', 'CVSCE', NULL, 'textedit', 'Окружность левой нижней конечности в средней трети голени', 'левой нижней в средней трети голени' UNION
SELECT 7860, 'CVSd', 'CVSall', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 7010, 'CVSPH', 'границы относительной и абсолютной тупости сердца в пределах нормы' UNION
SELECT 7020, 'CVSPH', 'границы относительной и абсолютной тупости сердца расширены вправо' UNION
SELECT 7030, 'CVSPH', 'границы относительной и абсолютной тупости сердца расширены влево' UNION
SELECT 7040, 'CVSPH', 'границы относительной и абсолютной тупости сердца расширены в обе стороны' UNION
SELECT 7050, 'CVSAH', 'сердечные тоны ясные, патологических шумов нет' UNION
SELECT 7060, 'CVSHB', 'ритмичные' UNION
SELECT 7070, 'CVSHB', 'аритмичные' UNION
SELECT 7080, 'CVSP', 'ритмичный' UNION
SELECT 7090, 'CVSP', 'аритмичный' UNION
SELECT 7100, 'CVSP', 'напряженный' UNION
SELECT 7110, 'CVSP', 'ненапряженный' UNION
SELECT 7120, 'CVSP', 'мягкий' UNION
SELECT 7130, 'CVSP', 'хорошего наполнения' UNION
SELECT 7140, 'CVSP', 'плохого наполнения' UNION
SELECT 7150, 'CVSP', 'большой' UNION
SELECT 7160, 'CVSP', 'малый' UNION
SELECT 7170, 'CVSP', 'нитевидный' UNION
SELECT 7180, 'CVSP', 'альтернирующий' UNION
SELECT 7190, 'CVSP', 'скорый' UNION
SELECT 7200, 'CVSP', 'медленный' UNION
SELECT 7210, 'CVSP', 'дикротический' UNION
SELECT 7220, 'CVSAP', 'на периферических артериях не определяется' UNION
SELECT 7230, 'CVSAT', 'определяется в 5 межреберье по левой срединно-ключичной линии' UNION
SELECT 7240, 'CVSAT', 'смещен влево' UNION
SELECT 7250, 'CVSAT', 'смещен вправо' UNION
SELECT 7260, 'CVSPM', 'не выслушивается' UNION
SELECT 7270, 'CVSPM', 'выслушивается' UNION
SELECT 7280, 'CVSCCAR', 'пульсация' UNION
SELECT 7290, 'CVSCCARP', 'обычная' UNION
SELECT 7300, 'CVSCCARP', 'усиленная' UNION
SELECT 7310, 'CVSCCARP', 'ослабленная' UNION
SELECT 7320, 'CVSCCARP', 'не пальпируется' UNION
SELECT 7330, 'CVSCCARN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 7340, 'CVSCCARN', 'аускультативно над артерией систолический шум' UNION
SELECT 7350, 'CVSCCAL', 'пульсация' UNION
SELECT 7360, 'CVSCCALP', 'обычная' UNION
SELECT 7370, 'CVSCCALP', 'усиленная' UNION
SELECT 7380, 'CVSCCALP', 'ослабленная' UNION
SELECT 7390, 'CVSCCALP', 'не пальпируется' UNION
SELECT 7400, 'CVSCCALN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 7410, 'CVSCCALN', 'аускультативно над артерией систолический шум' UNION
SELECT 7420, 'CVSCAR', 'шумы' UNION
SELECT 7430, 'CVSCARN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 7440, 'CVSCARN', 'аускультативно над артерией систолический шум' UNION
SELECT 7450, 'CVSCAL', 'шумы' UNION
SELECT 7460, 'CVSCALN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 7470, 'CVSCALN', 'аускультативно над артерией систолический шум' UNION
SELECT 7480, 'CVSBAR', 'пульсация' UNION
SELECT 7490, 'CVSBARP', 'обычная' UNION
SELECT 7500, 'CVSBARP', 'усиленная' UNION
SELECT 7510, 'CVSBARP', 'ослабленная' UNION
SELECT 7520, 'CVSBARP', 'не пальпируется' UNION
SELECT 7530, 'CVSBARN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 7540, 'CVSBARN', 'аускультативно над артерией систолический шум' UNION
SELECT 7550, 'CVSBAL', 'пульсация' UNION
SELECT 7560, 'CVSBALP', 'обычная' UNION
SELECT 7570, 'CVSBALP', 'усиленная' UNION
SELECT 7580, 'CVSBALP', 'ослабленная' UNION
SELECT 7590, 'CVSBALP', 'не пальпируется' UNION
SELECT 7600, 'CVSBALN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 7610, 'CVSBALN', 'аускультативно над артерией систолический шум' UNION
SELECT 7620, 'CVSRAR', 'пульсация' UNION
SELECT 7630, 'CVSRARP', 'обычная' UNION
SELECT 7640, 'CVSRARP', 'усиленная' UNION
SELECT 7650, 'CVSRARP', 'ослабленная' UNION
SELECT 7660, 'CVSRARP', 'не пальпируется' UNION
SELECT 7670, 'CVSRAL', 'пульсация' UNION
SELECT 7680, 'CVSRALP', 'обычная' UNION
SELECT 7690, 'CVSRALP', 'усиленная' UNION
SELECT 7700, 'CVSRALP', 'ослабленная' UNION
SELECT 7710, 'CVSRALP', 'не пальпируется' UNION
SELECT 7720, 'CVSAA', 'пульсация' UNION
SELECT 7730, 'CVSAAP', 'обычная' UNION
SELECT 7740, 'CVSAAP', 'усиленная' UNION
SELECT 7750, 'CVSAAP', 'ослабленная' UNION
SELECT 7760, 'CVSAAP', 'не пальпируется' UNION
SELECT 7770, 'CVSAAN', 'аускультативно патологических шумов в проекции аорты нет' UNION
SELECT 7780, 'CVSAAN', 'аускультативно над аортой систолический шум' UNION
SELECT 7790, 'CVSFAR', 'пульсация' UNION
SELECT 7800, 'CVSFARP', 'обычная' UNION
SELECT 7810, 'CVSFARP', 'усиленная' UNION
SELECT 7820, 'CVSFARP', 'ослабленная' UNION
SELECT 7830, 'CVSFARP', 'не пальпируется' UNION
SELECT 7840, 'CVSFARN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 7850, 'CVSFARN', 'аускультативно над артерией систолический шум' UNION
SELECT 7860, 'CVSFAL', 'пульсация' UNION
SELECT 7870, 'CVSFALP', 'обычная' UNION
SELECT 7880, 'CVSFALP', 'усиленная' UNION
SELECT 7890, 'CVSFALP', 'ослабленная' UNION
SELECT 7900, 'CVSFALP', 'не пальпируется' UNION
SELECT 7910, 'CVSFALN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 7920, 'CVSFALN', 'аускультативно над артерией систолический шум' UNION
SELECT 7930, 'CVSPAR', 'пульсация' UNION
SELECT 7940, 'CVSPARP', 'обычная' UNION
SELECT 7950, 'CVSPARP', 'усиленная' UNION
SELECT 7960, 'CVSPARP', 'ослабленная' UNION
SELECT 7970, 'CVSPARP', 'не пальпируется' UNION
SELECT 7980, 'CVSPARN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 7990, 'CVSPARN', 'аускультативно над артерией систолический шум' UNION
SELECT 8000, 'CVSPAL', 'пульсация' UNION
SELECT 8010, 'CVSPALP', 'обычная' UNION
SELECT 8020, 'CVSPALP', 'усиленная' UNION
SELECT 8030, 'CVSPALP', 'ослабленная' UNION
SELECT 8040, 'CVSPALP', 'не пальпируется' UNION
SELECT 8050, 'CVSPALN', 'аускультативно патологических шумов в проекции артерии нет' UNION
SELECT 8060, 'CVSPALN', 'аускультативно над артерией систолический шум' UNION
SELECT 8070, 'CVSRFAR', 'пульсация' UNION
SELECT 8080, 'CVSRFARP', 'обычная' UNION
SELECT 8090, 'CVSRFARP', 'усиленная' UNION
SELECT 8100, 'CVSRFARP', 'ослабленная' UNION
SELECT 8110, 'CVSRFARP', 'не пальпируется' UNION
SELECT 8120, 'CVSRFAL', 'пульсация' UNION
SELECT 8130, 'CVSRFALP', 'обычная' UNION
SELECT 8140, 'CVSRFALP', 'усиленная' UNION
SELECT 8150, 'CVSRFALP', 'ослабленная' UNION
SELECT 8160, 'CVSRFALP', 'не пальпируется' UNION
SELECT 8170, 'CVSPTAR', 'пульсация' UNION
SELECT 8180, 'CVSPTARP', 'обычная' UNION
SELECT 8190, 'CVSPTARP', 'усиленная' UNION
SELECT 8200, 'CVSPTARP', 'ослабленная' UNION
SELECT 8210, 'CVSPTARP', 'не пальпируется' UNION
SELECT 8220, 'CVSPTAL', 'пульсация' UNION
SELECT 8230, 'CVSPTALP', 'обычная' UNION
SELECT 8240, 'CVSPTALP', 'усиленная' UNION
SELECT 8250, 'CVSPTALP', 'ослабленная' UNION
SELECT 8260, 'CVSPTALP', 'не пальпируется';


----------------------------
-- Неврологический статус --
----------------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 9010, 'NSall', 'main', CAST(NULL AS "numeric"), 'container', 'Неврологический статус', NULL UNION
SELECT 9020, 'NS_os', 'NSall', NULL, 'container', 'Общемозговые симптомы', NULL UNION
SELECT 9030, 'NS_sud', 'NS_os', NULL, 'combobox', 'Судороги', NULL UNION
SELECT 9040, 'NS_ha', 'NS_os', NULL, 'textedit', 'Головная боль', NULL UNION
SELECT 9050, 'NS_go', 'NS_os', NULL, 'textedit', 'Головокружение', NULL UNION
SELECT 9060, 'NS_rv', 'NS_os', NULL, 'textedit', 'Рвота без предшествующего периода тошноты', NULL UNION
SELECT 9070, 'NS_cnzz', 'NS_os', NULL, 'textedit', 'Зрительные галлюцинации', NULL UNION
SELECT 9080, 'NS_cnsg', 'NS_os', NULL, 'textedit', 'Слуховые галлюцинации', NULL UNION
SELECT 9090, 'NS_cnog', 'NS_os', NULL, 'textedit', 'Обонятельные галлюцинации', NULL UNION
SELECT 9100, 'NS_cnosg', 'NS_os', NULL, 'textedit', 'Осязательные галлюцинации', NULL UNION
SELECT 9110, 'NS_osd', 'NS_os', NULL, 'textedit', 'Общемозговые симптомы (дополнительно)', 'дополнительно' UNION
SELECT 9120, 'NS_ms', 'NSall', NULL, 'container', 'Менингеальные симптомы', NULL UNION
SELECT 9130, 'NS_mp', 'NS_ms', NULL, 'textedit', 'Менингеальная поза', NULL UNION
SELECT 9140, 'NS_hy', 'NS_ms', NULL, 'textedit', 'Общая кожная гиперэстезия', NULL UNION
SELECT 9150, 'NS_rig', 'NS_ms', NULL, 'combobox', 'Ригидность затылочных мышц', NULL UNION
SELECT 9160, 'NS_syk', 'NS_ms', NULL, 'combobox', 'Симптом Кернига', NULL UNION
SELECT 9170, 'NS_syb', 'NS_ms', NULL, 'container', 'Симптом Брудзинского', NULL UNION
SELECT 9180, 'NS_syb_1d', 'NS_syb', NULL, 'combobox', 'Симптом Брудзинского - верхний', 'верхний' UNION
SELECT 9190, 'NS_syb_2d', 'NS_syb', NULL, 'combobox', 'Симптом Брудзинского - скуловой', 'скуловой' UNION
SELECT 9200, 'NS_syb_3d', 'NS_syb', NULL, 'combobox', 'Симптом Брудзинского - щечный', 'щечный' UNION
SELECT 9210, 'NS_syb_4d', 'NS_syb', NULL, 'combobox', 'Симптом Брудзинского - лобковый', 'лобковый' UNION
SELECT 9220, 'NS_syb_5d', 'NS_syb', NULL, 'combobox', 'Симптом Брудзинского - нижний', 'нижний' UNION
SELECT 9230, 'NS_syg', 'NS_ms', NULL, 'combobox', 'Симптом Гиллена', NULL UNION
SELECT 9240, 'NS_syl', 'NS_ms', NULL, 'combobox', 'Симптом подвешивания Лессажа', NULL UNION
SELECT 9250, 'NS_sybech', 'NS_ms', NULL, 'combobox', 'Симптом Бехтерева', NULL UNION
SELECT 9260, 'NS_sytre', 'NS_ms', NULL, 'combobox', 'Симптом "треножника"', NULL UNION
SELECT 9270, 'NS_syfan', 'NS_ms', NULL, 'combobox', 'Симптом Фанкони', NULL UNION
SELECT 9280, 'NS_sypoc', 'NS_ms', NULL, 'combobox', 'Симптом "поцелуя в колено"', NULL UNION
SELECT 9290, 'NS_sym', 'NS_ms', NULL, 'combobox', 'Симптом Мэйтуса', NULL UNION
SELECT 9300, 'NS_syr', 'NS_ms', NULL, 'textedit', 'Большой родничок', NULL UNION
SELECT 9310, 'NS_msd', 'NS_ms', NULL, 'textedit', 'Менингеальные симптомы (дополнительно)', 'дополнительно' UNION
SELECT 9320, 'NS_cn', 'NSall', NULL, 'container', 'Черепные нервы', NULL UNION
SELECT 9330, 'NS_cno', 'NS_cn', NULL, 'container', 'Обонятельный нерв', NULL UNION
SELECT 9340, 'NS_cnoan', 'NS_cno', NULL, 'textedit', 'Аносмия', NULL UNION
SELECT 9350, 'NS_cnohy', 'NS_cno', NULL, 'textedit', 'Гипосмия', NULL UNION
SELECT 9360, 'NS_cnohyp', 'NS_cno', NULL, 'textedit', 'Гиперосмия', NULL UNION
SELECT 9370, 'NS_cnodys', 'NS_cno', NULL, 'textedit', 'Дизосмия', NULL UNION
SELECT 9380, 'NS_cnod', 'NS_cno', NULL, 'textedit', 'Обонятельный нерв (дополнительно)', 'дополнительно' UNION
SELECT 9390, 'NS_cnz', 'NS_cn', NULL, 'container', 'Зрительный нерв', NULL UNION
SELECT 9400, 'NS_cnzg', 'NS_cnz', NULL, 'textedit', 'Глазное дно', NULL UNION
SELECT 9410, 'NS_cnzd', 'NS_cnz', NULL, 'textedit', 'Зрительный нерв (дополнительно)', 'дополнительно' UNION
SELECT 9420, 'NS_cng', 'NS_cn', NULL, 'container', 'Глазодвигательные нервы', NULL UNION
SELECT 9430, 'NS_cngs', 'NS_cng', NULL, 'textedit', 'Ширина и равномерность глазных щелей', NULL UNION
SELECT 9440, 'NS_cngr', 'NS_cng', NULL, 'textedit', 'Положение и объем движений глазных яблок', NULL UNION
SELECT 9450, 'NS_cngb', 'NS_cng', NULL, 'combobox', 'Синдром Бернара-Горнера', NULL UNION
SELECT 9460, 'NS_cnga', 'NS_cng', NULL, 'combobox', 'Синдром Аргайла-Робертсона', NULL UNION
SELECT 9470, 'NS_cngg', 'NS_cng', NULL, 'combobox', 'Синдром Гертвига-Мажанди', NULL UNION
SELECT 9480, 'NS_cngdop', 'NS_cng', NULL, 'textedit', 'Глазодвигательные нервы (дополнительно)', 'дополнительно' UNION
SELECT 9490, 'NS_cnt', 'NS_cn', NULL, 'container', 'Тройничный нерв', NULL UNION
SELECT 9500, 'NS_cntm', 'NS_cnt', NULL, 'textedit', 'Объем движений нижней челюсти', NULL UNION
SELECT 9510, 'NS_cntz', 'NS_cnt', NULL, 'textedit', 'Напряжение и трофика жевательных мышц', NULL UNION
SELECT 9520, 'NS_cntb', 'NS_cnt', NULL, 'textedit', 'Боли и парестезии в области лица', NULL UNION
SELECT 9530, 'NS_cntt', 'NS_cnt', NULL, 'textedit', 'Болевые точки', NULL UNION
SELECT 9540, 'NS_cntc', 'NS_cnt', NULL, 'textedit', 'Чувствительность кожи лица', NULL UNION
SELECT 9550, 'NS_cntrn', 'NS_cnt', NULL, 'combobox', 'Надбровный рефлекс', NULL UNION
SELECT 9560, 'NS_cntrc', 'NS_cnt', NULL, 'combobox', 'Конъюнктивальный рефлекс', NULL UNION
SELECT 9570, 'NS_cntrk', 'NS_cnt', NULL, 'combobox', 'Корнеальный рефлекс', NULL UNION
SELECT 9580, 'NS_cntrm', 'NS_cnt', NULL, 'combobox', 'Нижнечелюстной рефлекс', NULL UNION
SELECT 9590, 'NS_cntd', 'NS_cnt', NULL, 'textedit', 'Тройничный нерв (дополнительно)', 'дополнительно' UNION
SELECT 9600, 'NS_cnl', 'NS_cn', NULL, 'container', 'Лицевой нерв', NULL UNION
SELECT 9610, 'NS_cnlm', 'NS_cnl', NULL, 'textedit', 'Иннервация мимической мускулатуры', NULL UNION
SELECT 9620, 'NS_cnlt', 'NS_cnl', NULL, 'textedit', 'Чувствительность передних 2/3 языка', NULL UNION
SELECT 9630, 'NS_cnlh', 'NS_cnl', NULL, 'textedit', 'Гиперакузия', NULL UNION
SELECT 9640, 'NS_cnls', 'NS_cnl', NULL, 'textedit', 'Слезотечение', NULL UNION
SELECT 9650, 'NS_cnlb', 'NS_cnl', NULL, 'combobox', 'Симптом Белла', NULL UNION
SELECT 9660, 'NS_cnlc', 'NS_cnl', NULL, 'combobox', 'Симптом "крокодиловых слез"', NULL UNION
SELECT 9670, 'NS_cnll', 'NS_cnl', NULL, 'textedit', 'Лагофтальм', NULL UNION
SELECT 9680, 'NS_cnld', 'NS_cnl', NULL, 'textedit', 'Лицевой нерв (дополнительно)', 'дополнительно' UNION
SELECT 9690, 'NS_cns', 'NS_cn', NULL, 'container', 'Слуховой и вестибулярный нервы', NULL UNION
SELECT 9700, 'NS_cnso', 'NS_cns', NULL, 'combobox', 'Острота слуха', NULL UNION
SELECT 9710, 'NS_cnsh', 'NS_cns', NULL, 'textedit', 'Гипакузия', NULL UNION
SELECT 9720, 'NS_cnsz', 'NS_cns', NULL, 'textedit', 'Гиперакузия', NULL UNION
SELECT 9730, 'NS_cnss', 'NS_cns', NULL, 'textedit', 'Шум в ушах', NULL UNION
SELECT 9740, 'NS_cnsn', 'NS_cns', NULL, 'textedit', 'Нистагм', NULL UNION
SELECT 9750, 'NS_cnsd', 'NS_cns', NULL, 'textedit', 'Слуховой и вестибулярный нервы (дополнительно)', 'дополнительно' UNION
SELECT 9760, 'NS_cny', 'NS_cn', NULL, 'container', 'Языкоглоточный и блуждающий нервы', NULL UNION
SELECT 9770, 'NS_cnym', 'NS_cny', NULL, 'textedit', 'Мягкое небо', NULL UNION
SELECT 9780, 'NS_cnyl', 'NS_cny', NULL, 'textedit', 'Положение язычка', NULL UNION
SELECT 9790, 'NS_cnyc', 'NS_cny', NULL, 'textedit', 'Чувствительность', NULL UNION
SELECT 9800, 'NS_cnyg', 'NS_cny', NULL, 'textedit', 'Глоточные рефлексы', NULL UNION
SELECT 9810, 'NS_cnyn', 'NS_cny', NULL, 'textedit', 'Небные рефлексы', NULL UNION
SELECT 9820, 'NS_cnyo', 'NS_cny', NULL, 'textedit', 'Глотание', NULL UNION
SELECT 9830, 'NS_cnyf', 'NS_cny', NULL, 'textedit', 'Фонация', NULL UNION
SELECT 9840, 'NS_cnya', 'NS_cny', NULL, 'textedit', 'Артикуляция', NULL UNION
SELECT 9850, 'NS_cnyi', 'NS_cny', NULL, 'textedit', 'Иннервация задней трети языка', NULL UNION
SELECT 9860, 'NS_cnys', 'NS_cny', NULL, 'textedit', 'Саливация', NULL UNION
SELECT 9870, 'NS_cnyp', 'NS_cny', NULL, 'textedit', 'Парасимпатическая иннервация внутренних органов', NULL UNION
SELECT 9880, 'NS_cnyd', 'NS_cny', NULL, 'textedit', 'Языкоглоточный и блуждающий нервы (дополнительно)', 'дополнительно' UNION
SELECT 9890, 'NS_cnd', 'NSall', NULL, 'container', 'Добавочный нерв', NULL UNION
SELECT 9900, 'NS_cndp', 'NS_cnd', NULL, 'textedit', 'Положение плеч, лопаток и головы в покое', NULL UNION
SELECT 9910, 'NS_cndm', 'NS_cnd', NULL, 'textedit', 'Поднимание плеч, повороты головы', NULL UNION
SELECT 9920, 'NS_cnds', 'NS_cnd', NULL, 'textedit', 'Напряжение, трофика грудино-ключично-сосцевидной мышцы и трапециевидной мышцы', NULL UNION
SELECT 9930, 'NS_cndk', 'NS_cnd', NULL, 'textedit', 'Кривошея', NULL UNION
SELECT 9940, 'NS_cndd', 'NS_cnd', NULL, 'textedit', 'Добавочный нерв (дополнительно)', 'дополнительно' UNION
SELECT 9950, 'NS_cnpod', 'NSall', NULL, 'container', 'Подъязычный нерв', NULL UNION
SELECT 9960, 'NS_cnpodg', 'NS_cnpod', NULL, 'textedit', 'Положение языка в полости рта и при высовывании', NULL UNION
SELECT 9970, 'NS_cnpoda', 'NS_cnpod', NULL, 'textedit', 'Атрофия (гипотрофия) и фибриллярные подергивания в мышцах языка', NULL UNION
SELECT 9980, 'NS_cnpodr', 'NS_cnpod', NULL, 'textedit', 'Дизартрия', NULL UNION
SELECT 9990, 'NS_cnpodd', 'NS_cnpod', NULL, 'textedit', 'Подъязычный нерв (дополнительно)', 'дополнительно' UNION
SELECT 10000, 'NS_bu', 'NSall', NULL, 'container', 'Бульбарный и псевдобульбарный синдромы', NULL UNION
SELECT 10010, 'NS_bud', 'NS_bu', NULL, 'textedit', 'Дизартрия', NULL UNION
SELECT 10020, 'NS_buf', 'NS_bu', NULL, 'textedit', 'Дисфония', NULL UNION
SELECT 10030, 'NS_bug', 'NS_bu', NULL, 'textedit', 'Дисфагия', NULL UNION
SELECT 10040, 'NS_bum', 'NS_bu', NULL, 'combobox', 'Рефлекс Маринеско-Радовичи', NULL UNION
SELECT 10050, 'NS_buv', 'NS_bu', NULL, 'combobox', 'Рефлекс Вюрпа', NULL UNION
SELECT 10060, 'NS_buo', 'NS_bu', NULL, 'combobox', 'Рефлекс Оппенгейма', NULL UNION
SELECT 10070, 'NS_bua', 'NS_bu', NULL, 'combobox', 'Рефлекс Аствацатурова', NULL UNION
SELECT 10080, 'NS_buc', 'NS_bu', NULL, 'combobox', 'Корнеоментальный рефлекс', NULL UNION
SELECT 10090, 'NS_buk', 'NS_bu', NULL, 'combobox', 'Корнеомандибулярный рефлекс', NULL UNION
SELECT 10100, 'NS_bus', 'NS_bu', NULL, 'combobox', 'Дистанс-рефлекс', NULL UNION
SELECT 10110, 'NS_budop', 'NS_bu', NULL, 'textedit', 'Бульбарный и псевдобульбарный синдромы (дополнительно)', 'дополнительно' UNION
SELECT 10120, 'NS_dv', 'NSall', NULL, 'container', 'Двигательная сфера', NULL UNION
SELECT 10130, 'NS_dvm', 'NS_dv', NULL, 'combobox', 'Проба Мингаццини-Барре', NULL UNION
SELECT 10140, 'NS_dvl', 'NS_dv', NULL, 'textedit', 'Параличи и парезы конечностей', NULL UNION
SELECT 10150, 'NS_dvd', 'NS_dv', NULL, 'textedit', 'Двигательная сфера (дополнительно)', 'дополнительно' UNION
SELECT 10160, 'NS_res', 'NSall', NULL, 'container', 'Сухожильные и надкостничные рефлексы', NULL UNION
SELECT 10170, 'NS_resm', 'NS_res', NULL, 'combobox', 'Нижнечелюстной рефлекс', NULL UNION
SELECT 10180, 'NS_resn', 'NS_res', NULL, 'combobox', 'Надбровный рефлекс', NULL UNION
SELECT 10190, 'NS_resb', 'NS_res', NULL, 'combobox', 'Рефлекс с двуглавой мыщцы плеча', NULL UNION
SELECT 10200, 'NS_rest', 'NS_res', NULL, 'combobox', 'Рефлекс с трехглавой мышцы плеча', NULL UNION
SELECT 10210, 'NS_resc', 'NS_res', NULL, 'combobox', 'Карпорадиальный рефлекс', NULL UNION
SELECT 10220, 'NS_resr', 'NS_res', NULL, 'combobox', 'Реберно-абдоминальный рефлекс', NULL UNION
SELECT 10230, 'NS_reso', 'NS_res', NULL, 'combobox', 'Лопаточно-плечевой рефлекс', NULL UNION
SELECT 10240, 'NS_resp', 'NS_res', NULL, 'combobox', 'Лобковый рефлекс', NULL UNION
SELECT 10250, 'NS_resk', 'NS_res', NULL, 'combobox', 'Коленный рефлекс', NULL UNION
SELECT 10260, 'NS_resa', 'NS_res', NULL, 'combobox', 'Ахиллов рефлекс', NULL UNION
SELECT 10270, 'NS_resv', 'NS_res', NULL, 'combobox', 'Подошвенный рефлекс', NULL UNION
SELECT 10280, 'NS_resd', 'NS_res', NULL, 'textedit', 'Сухожильные и надкостничные рефлексы (дополнительно)', 'дополнительно' UNION
SELECT 10290, 'NS_rec', 'NSall', NULL, 'container', 'Кожные рефлексы', NULL UNION
SELECT 10300, 'NS_reca', 'NS_rec', NULL, 'textedit', 'Брюшные рефлексы', NULL UNION
SELECT 10310, 'NS_recc', 'NS_rec', NULL, 'textedit', 'Кремастерные рефлексы', NULL UNION
SELECT 10320, 'NS_recn', 'NS_rec', NULL, 'textedit', 'Анальный рефлекс', NULL UNION
SELECT 10330, 'NS_recy', 'NS_rec', NULL, 'textedit', 'Ягодичный рефлекс', NULL UNION
SELECT 10340, 'NS_recp', 'NS_rec', NULL, 'textedit', 'Подошвенный рефлекс', NULL UNION
SELECT 10350, 'NS_recd', 'NS_rec', NULL, 'textedit', 'Кожные рефлексы (дополнительно)', 'дополнительно' UNION
SELECT 10360, 'NS_resl', 'NSall', NULL, 'container', 'Рефлексы со слизистых', NULL UNION
SELECT 10370, 'NS_reslc', 'NS_resl', NULL, 'combobox', 'Конъюнктивальный рефлекс', NULL UNION
SELECT 10380, 'NS_reslo', 'NS_resl', NULL, 'combobox', 'Корнеальный рефлекс', NULL UNION
SELECT 10390, 'NS_reslg', 'NS_resl', NULL, 'combobox', 'Глоточный рефлекс', NULL UNION
SELECT 10400, 'NS_resln', 'NS_resl', NULL, 'combobox', 'Небный рефлекс', NULL UNION
SELECT 10410, 'NS_resld', 'NS_resl', NULL, 'textedit', 'Рефлексы со слизистых (дополнительно)', 'дополнительно' UNION
SELECT 10420, 'NS_rep', 'NSall', NULL, 'container', 'Патологические рефлексы', NULL UNION
SELECT 10430, 'NS_repr', 'NS_rep', NULL, 'textedit', 'Кистевой рефлекс Россолимо', NULL UNION
SELECT 10440, 'NS_repj', 'NS_rep', NULL, 'textedit', 'Кистевой рефлекс Жуковского', NULL UNION
SELECT 10450, 'NS_repi', 'NS_rep', NULL, 'textedit', 'Кистевой рефлекс Якобсона-Ласка', NULL UNION
SELECT 10460, 'NS_reps', 'NS_rep', NULL, 'textedit', 'Стопный рефлекс Россолимо', NULL UNION
SELECT 10470, 'NS_repj2', 'NS_rep', NULL, 'textedit', 'Стопный рефлекс Жуковского', NULL UNION
SELECT 10480, 'NS_repb1', 'NS_rep', NULL, 'textedit', 'Стопный рефлекс Бехтерева-1', NULL UNION
SELECT 10490, 'NS_repb2', 'NS_rep', NULL, 'textedit', 'Стопный рефлекс Бехтерева-2', NULL UNION
SELECT 10500, 'NS_repba', 'NS_rep', NULL, 'textedit', 'Стопный рефлекс Бабинского', NULL UNION
SELECT 10510, 'NS_repo', 'NS_rep', NULL, 'textedit', 'Стопный рефлекс Оппенгейма', NULL UNION
SELECT 10520, 'NS_repg', 'NS_rep', NULL, 'textedit', 'Стопный рефлекс Гордона', NULL UNION
SELECT 10530, 'NS_reph', 'NS_rep', NULL, 'textedit', 'Стопный рефлекс Шеффера', NULL UNION
SELECT 10540, 'NS_repbe', 'NS_rep', NULL, 'textedit', 'Симптом Бехтерева-Мари-Фуа', NULL UNION
SELECT 10550, 'NS_repsh', 'NS_rep', NULL, 'textedit', 'Тибиальный феномен Штрюмпелля', NULL UNION
SELECT 10560, 'NS_repp', 'NS_rep', NULL, 'textedit', 'Пронаторная синкинезия', NULL UNION
SELECT 10570, 'NS_repra', 'NS_rep', NULL, 'textedit', 'Радиальная синкинезия', NULL UNION
SELECT 10580, 'NS_repsy', 'NS_rep', NULL, 'textedit', 'Синкинезия Раймиста', NULL UNION
SELECT 10590, 'NS_repd', 'NS_rep', NULL, 'textedit', 'Патологические рефлексы (дополнительно)', 'дополнительно' UNION
SELECT 10600, 'NS_co', 'NSall', NULL, 'container', 'Координация движений', NULL UNION
SELECT 10610, 'NS_coa', 'NS_co', NULL, 'textedit', 'Атаксия', NULL UNION
SELECT 10620, 'NS_cot', 'NS_co', NULL, 'textedit', 'Тремор', NULL UNION
SELECT 10630, 'NS_con', 'NS_co', NULL, 'textedit', 'Нистагм', NULL UNION
SELECT 10640, 'NS_cos', 'NS_co', NULL, 'textedit', 'Скандированная речь', NULL UNION
SELECT 10650, 'NS_cor', 'NS_co', NULL, 'textedit', 'Проба Ромберга', NULL UNION
SELECT 10660, 'NS_copn', 'NS_co', NULL, 'textedit', 'Локомоторная пальценосовая проба', NULL UNION
SELECT 10670, 'NS_copk', 'NS_co', NULL, 'textedit', 'Локомоторная пяточноколенная проба', NULL UNION
SELECT 10680, 'NS_cosh', 'NS_co', NULL, 'combobox', 'Проба Шильдера', NULL UNION
SELECT 10690, 'NS_coi', 'NS_co', NULL, 'textedit', 'Пробы на адиадохокинез', NULL UNION
SELECT 10700, 'NS_coas', 'NS_co', NULL, 'textedit', 'Пробы на асинергию', NULL UNION
SELECT 10710, 'NS_cohy', 'NS_co', NULL, 'textedit', 'Пробы на гиперметрию', NULL UNION
SELECT 10720, 'NS_cost', 'NS_co', NULL, 'combobox', 'Феномен Стюарта-Холмса', NULL UNION
SELECT 10730, 'NS_cod', 'NS_co', NULL, 'textedit', 'Координация движений (дополнительно)', 'дополнительно' UNION
SELECT 10740, 'NS_chu', 'NSall', NULL, 'container', 'Чувствительность', NULL UNION
SELECT 10750, 'NS_chub', 'NS_chu', NULL, 'textedit', 'Болевая чувствительность', NULL UNION
SELECT 10760, 'NS_chut', 'NS_chu', NULL, 'textedit', 'Температурная чувствительность', NULL UNION
SELECT 10770, 'NS_chuta', 'NS_chu', NULL, 'textedit', 'Тактильная чувствительность', NULL UNION
SELECT 10780, 'NS_chul', 'NS_chu', NULL, 'combobox', 'Симптом Лассега', NULL UNION
SELECT 10790, 'NS_chun', 'NS_chu', NULL, 'combobox', 'Симптом Нери', NULL UNION
SELECT 10800, 'NS_chus', 'NS_chu', NULL, 'combobox', 'Симптом Сикара', NULL UNION
SELECT 10810, 'NS_chum', 'NS_chu', NULL, 'combobox', 'Симптом Мацкевича', NULL UNION
SELECT 10820, 'NS_chuv', 'NS_chu', NULL, 'combobox', 'Симптом Вассермана', NULL UNION
SELECT 10830, 'NS_chubt', 'NS_chu', NULL, 'textedit', 'Болевые точки', NULL UNION
SELECT 10840, 'NS_chuz', 'NS_chu', NULL, 'textedit', 'Зоны Захарьина-Геда', NULL UNION
SELECT 10850, 'NS_chug', 'NS_chu', NULL, 'textedit', 'Глубокая чувствительность', NULL UNION
SELECT 10860, 'NS_chud', 'NS_chu', NULL, 'textedit', 'Чувствительность (дополнительно)', 'дополнительно' UNION
SELECT 10870, 'NS_ve', 'NSall', NULL, 'container', 'Вегетативно-трофическая сфера', NULL UNION
SELECT 10880, 'NS_vet', 'NS_ve', NULL, 'textedit', 'Трофика кожи', NULL UNION
SELECT 10890, 'NS_ven', 'NS_ve', NULL, 'textedit', 'Состояние ногтей и оволосение', NULL UNION
SELECT 10900, 'NS_vee', 'NS_ve', NULL, 'textedit', 'Дермографизм', NULL UNION
SELECT 10910, 'NS_ves', 'NS_ve', NULL, 'textedit', 'Солярный рефлекс', NULL UNION
SELECT 10920, 'NS_vec', 'NS_ve', NULL, 'textedit', 'Клиноортостатическая проба', NULL UNION
SELECT 10930, 'NS_veg', 'NS_ve', NULL, 'textedit', 'Глазосердечный рефлекс Даньини-Ашнера', NULL UNION
SELECT 10940, 'NS_vepi', 'NS_ve', NULL, 'textedit', 'Пиломоторный рефлекс', NULL UNION
SELECT 10950, 'NS_ved', 'NS_ve', NULL, 'textedit', 'Вегетативно-трофическая сфера (дополнительно)', 'дополнительно' UNION
SELECT 10960, 'NS_vi', 'NSall', NULL, 'container', 'Высшие корковые функции', NULL UNION
SELECT 10970, 'NS_vig', 'NS_vi', NULL, 'textedit', 'Гнозис', NULL UNION
SELECT 10980, 'NS_vip', 'NS_vi', NULL, 'textedit', 'Праксис', NULL UNION
SELECT 10990, 'NS_vir', 'NS_vi', NULL, 'textedit', 'Речь', NULL UNION
SELECT 11000, 'NS_vis', 'NS_vi', NULL, 'textedit', 'Письмо, чтение, счет', NULL UNION
SELECT 11010, 'NS_viv', 'NS_vi', NULL, 'textedit', 'Память, внимание, интеллект', NULL UNION
SELECT 11020, 'NS_vie', 'NS_vi', NULL, 'textedit', 'Эмоциональная сфера', NULL UNION
SELECT 11030, 'NS_viso', 'NS_vi', NULL, 'textedit', 'Сон', NULL UNION
SELECT 11040, 'NS_vid', 'NS_vi', NULL, 'textedit', 'Высшие корковые функции (дополнительно)', 'дополнительно' UNION
SELECT 11050, 'NS_d', 'NSall', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 9010, 'NS_sud', 'отсутствуют' UNION
SELECT 9020, 'NS_sud', 'наблюдаются парциальные судороги' UNION
SELECT 9030, 'NS_sud', 'наблюдаются генерализованные судороги' UNION
SELECT 9040, 'NS_sud', 'наблюдаются тонические судороги' UNION
SELECT 9050, 'NS_sud', 'наблюдаются клонические судороги' UNION
SELECT 9060, 'NS_sud', 'наблюдаются тонико-клонические судороги' UNION
SELECT 9070, 'NS_rig', 'не выявляется' UNION
SELECT 9080, 'NS_rig', 'выраженная' UNION
SELECT 9090, 'NS_rig', 'умеренная' UNION
SELECT 9100, 'NS_syk', 'отрицательный' UNION
SELECT 9110, 'NS_syk', 'положительный' UNION
SELECT 9120, 'NS_syk', 'резко положительный' UNION
SELECT 9130, 'NS_syb_1d', 'отрицательный' UNION
SELECT 9140, 'NS_syb_1d', 'положительный' UNION
SELECT 9150, 'NS_syb_1d', 'резко положительный' UNION
SELECT 9160, 'NS_syb_2d', 'отрицательный' UNION
SELECT 9170, 'NS_syb_2d', 'положительный' UNION
SELECT 9180, 'NS_syb_2d', 'резко положительный' UNION
SELECT 9190, 'NS_syb_3d', 'отрицательный' UNION
SELECT 9200, 'NS_syb_3d', 'положительный' UNION
SELECT 9210, 'NS_syb_3d', 'резко положительный' UNION
SELECT 9220, 'NS_syb_4d', 'отрицательный' UNION
SELECT 9230, 'NS_syb_4d', 'положительный' UNION
SELECT 9240, 'NS_syb_4d', 'резко положительный' UNION
SELECT 9250, 'NS_syb_5d', 'отрицательный' UNION
SELECT 9260, 'NS_syb_5d', 'положительный' UNION
SELECT 9270, 'NS_syb_5d', 'резко положительный' UNION
SELECT 9280, 'NS_syg', 'отрицательный' UNION
SELECT 9290, 'NS_syg', 'положительный' UNION
SELECT 9300, 'NS_syg', 'резко положительный' UNION
SELECT 9310, 'NS_syl', 'отрицательный' UNION
SELECT 9320, 'NS_syl', 'положительный' UNION
SELECT 9330, 'NS_sybech', 'отрицательный' UNION
SELECT 9340, 'NS_sybech', 'положительный' UNION
SELECT 9350, 'NS_sytre', 'отрицательный' UNION
SELECT 9360, 'NS_sytre', 'положительный' UNION
SELECT 9370, 'NS_syfan', 'отрицательный' UNION
SELECT 9380, 'NS_syfan', 'положительный' UNION
SELECT 9390, 'NS_sypoc', 'отрицательный' UNION
SELECT 9400, 'NS_sypoc', 'положительный' UNION
SELECT 9410, 'NS_sym', 'отрицательный' UNION
SELECT 9420, 'NS_sym', 'положительный' UNION
SELECT 9430, 'NS_cngb', 'отрицательный' UNION
SELECT 9440, 'NS_cngb', 'положительный' UNION
SELECT 9450, 'NS_cngb', 'резко положительный' UNION
SELECT 9460, 'NS_cnga', 'отрицательный' UNION
SELECT 9470, 'NS_cnga', 'положительный' UNION
SELECT 9480, 'NS_cngg', 'отрицательный' UNION
SELECT 9490, 'NS_cngg', 'положительный' UNION
SELECT 9500, 'NS_cntrn', 'обычный' UNION
SELECT 9510, 'NS_cntrn', 'оживлен' UNION
SELECT 9520, 'NS_cntrn', 'вялый' UNION
SELECT 9530, 'NS_cntrn', 'отсутствует' UNION
SELECT 9540, 'NS_cntrc', 'обычный' UNION
SELECT 9550, 'NS_cntrc', 'оживлен' UNION
SELECT 9560, 'NS_cntrc', 'вялый' UNION
SELECT 9570, 'NS_cntrc', 'отсутствует' UNION
SELECT 9580, 'NS_cntrk', 'обычный' UNION
SELECT 9590, 'NS_cntrk', 'оживлен' UNION
SELECT 9600, 'NS_cntrk', 'вялый' UNION
SELECT 9610, 'NS_cntrk', 'отсутствует' UNION
SELECT 9620, 'NS_cntrm', 'обычный' UNION
SELECT 9630, 'NS_cntrm', 'оживлен' UNION
SELECT 9640, 'NS_cntrm', 'вялый' UNION
SELECT 9650, 'NS_cntrm', 'отсутствует' UNION
SELECT 9660, 'NS_cnlb', 'отрицательный' UNION
SELECT 9670, 'NS_cnlb', 'положительный' UNION
SELECT 9680, 'NS_cnlc', 'отрицательный' UNION
SELECT 9690, 'NS_cnlc', 'положительный' UNION
SELECT 9700, 'NS_cnso', 'не изменена' UNION
SELECT 9710, 'NS_cnso', 'снижена' UNION
SELECT 9720, 'NS_bum', 'отрицательный' UNION
SELECT 9730, 'NS_bum', 'положительный' UNION
SELECT 9740, 'NS_buv', 'отрицательный' UNION
SELECT 9750, 'NS_buv', 'положительный' UNION
SELECT 9760, 'NS_buo', 'отрицательный' UNION
SELECT 9770, 'NS_buo', 'положительный' UNION
SELECT 9780, 'NS_bua', 'отрицательный' UNION
SELECT 9790, 'NS_bua', 'положительный' UNION
SELECT 9800, 'NS_buc', 'отрицательный' UNION
SELECT 9810, 'NS_buc', 'положительный' UNION
SELECT 9820, 'NS_buk', 'отрицательный' UNION
SELECT 9830, 'NS_buk', 'положительный' UNION
SELECT 9840, 'NS_bus', 'отрицательный' UNION
SELECT 9850, 'NS_bus', 'положительный' UNION
SELECT 9860, 'NS_dvm', 'отрицательная' UNION
SELECT 9870, 'NS_dvm', 'положительная' UNION
SELECT 9880, 'NS_resm', 'обычный' UNION
SELECT 9890, 'NS_resm', 'оживлен' UNION
SELECT 9900, 'NS_resm', 'вялый' UNION
SELECT 9910, 'NS_resm', 'отсутствует' UNION
SELECT 9920, 'NS_resn', 'обычный' UNION
SELECT 9930, 'NS_resn', 'оживлен' UNION
SELECT 9940, 'NS_resn', 'вялый' UNION
SELECT 9950, 'NS_resn', 'отсутствует' UNION
SELECT 9960, 'NS_resb', 'обычный' UNION
SELECT 9970, 'NS_resb', 'оживлен' UNION
SELECT 9980, 'NS_resb', 'вялый' UNION
SELECT 9990, 'NS_resb', 'отсутствует' UNION
SELECT 10000, 'NS_rest', 'обычный' UNION
SELECT 10010, 'NS_rest', 'оживлен' UNION
SELECT 10020, 'NS_rest', 'вялый' UNION
SELECT 10030, 'NS_rest', 'отсутствует' UNION
SELECT 10040, 'NS_resc', 'обычный' UNION
SELECT 10050, 'NS_resc', 'оживлен' UNION
SELECT 10060, 'NS_resc', 'вялый' UNION
SELECT 10070, 'NS_resc', 'отсутствует' UNION
SELECT 10080, 'NS_resr', 'обычный' UNION
SELECT 10090, 'NS_resr', 'оживлен' UNION
SELECT 10100, 'NS_resr', 'вялый' UNION
SELECT 10110, 'NS_resr', 'отсутствует' UNION
SELECT 10120, 'NS_reso', 'обычный' UNION
SELECT 10130, 'NS_reso', 'оживлен' UNION
SELECT 10140, 'NS_reso', 'вялый' UNION
SELECT 10150, 'NS_reso', 'отсутствует' UNION
SELECT 10160, 'NS_resp', 'обычный' UNION
SELECT 10170, 'NS_resp', 'оживлен' UNION
SELECT 10180, 'NS_resp', 'вялый' UNION
SELECT 10190, 'NS_resp', 'отсутствует' UNION
SELECT 10200, 'NS_resk', 'обычный' UNION
SELECT 10210, 'NS_resk', 'оживлен' UNION
SELECT 10220, 'NS_resk', 'вялый' UNION
SELECT 10230, 'NS_resk', 'отсутствует' UNION
SELECT 10240, 'NS_resa', 'обычный' UNION
SELECT 10250, 'NS_resa', 'оживлен' UNION
SELECT 10260, 'NS_resa', 'вялый' UNION
SELECT 10270, 'NS_resa', 'отсутствует' UNION
SELECT 10280, 'NS_resv', 'обычный' UNION
SELECT 10290, 'NS_resv', 'оживлен' UNION
SELECT 10300, 'NS_resv', 'вялый' UNION
SELECT 10310, 'NS_resv', 'отсутствует' UNION
SELECT 10320, 'NS_reslc', 'положительный' UNION
SELECT 10330, 'NS_reslc', 'отрицательный' UNION
SELECT 10340, 'NS_reslo', 'положительный' UNION
SELECT 10350, 'NS_reslo', 'отрицательный' UNION
SELECT 10360, 'NS_reslg', 'положительный' UNION
SELECT 10370, 'NS_reslg', 'отрицательный' UNION
SELECT 10380, 'NS_resln', 'положительный' UNION
SELECT 10390, 'NS_resln', 'отрицательный' UNION
SELECT 10400, 'NS_cosh', 'положительный' UNION
SELECT 10410, 'NS_cosh', 'отрицательный' UNION
SELECT 10420, 'NS_cost', 'положительный' UNION
SELECT 10430, 'NS_cost', 'отрицательный' UNION
SELECT 10440, 'NS_chul', 'положительный' UNION
SELECT 10450, 'NS_chul', 'отрицательный' UNION
SELECT 10460, 'NS_chun', 'положительный' UNION
SELECT 10470, 'NS_chun', 'отрицательный' UNION
SELECT 10480, 'NS_chus', 'положительный' UNION
SELECT 10490, 'NS_chus', 'отрицательный' UNION
SELECT 10500, 'NS_chum', 'положительный' UNION
SELECT 10510, 'NS_chum', 'отрицательный' UNION
SELECT 10520, 'NS_chuv', 'положительный' UNION
SELECT 10530, 'NS_chuv', 'отрицательный';


------------------------
-- Органы пищеварения --
------------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 12010, 'DAall', 'main', CAST(NULL AS "numeric"), 'container', 'Органы пищеварения', NULL UNION
SELECT 12020, 'DAMOC', 'DAall', NULL, 'textedit', 'Рот и полость рта', NULL UNION
SELECT 12030, 'DADH', 'DAall', NULL, 'textedit', 'Состояние зубов', NULL UNION
SELECT 12040, 'DAT', 'DAall', NULL, 'combobox', 'Язык', NULL UNION
SELECT 12050, 'DAZ', 'DAall', NULL, 'textedit', 'Зев', NULL UNION
SELECT 12060, 'DATT', 'DAall', NULL, 'combobox', 'Миндалины', NULL UNION
SELECT 12070, 'DAA', 'DAall', NULL, 'combobox', 'Живот', NULL UNION
SELECT 12080, 'DAAJ', 'DAall', NULL, 'combobox', 'Тонкая кишка', NULL UNION
SELECT 12090, 'DAAC', 'DAall', NULL, 'combobox', 'Толстая кишка', NULL UNION
SELECT 12100, 'DAAS', 'DAall', NULL, 'combobox', 'Сигмовидная кишка', NULL UNION
SELECT 12110, 'DAAd4', 'DAall', NULL, 'textedit', 'Окружность живота на уровне пупка', NULL UNION
SELECT 12120, 'NS_to', 'DAall', NULL, 'textedit', 'Тошнота', NULL UNION
SELECT 12130, 'DASlog', 'DAall', NULL, 'container', 'Симптомы', NULL UNION
SELECT 12140, 'DASVA', 'DASlog', NULL, 'combobox', 'Симптом Волковича аппендикулярный', NULL UNION
SELECT 12150, 'DASM', 'DASlog', NULL, 'combobox', 'Симптом Менделя', NULL UNION
SELECT 12160, 'DASR', 'DASlog', NULL, 'combobox', 'Симптом Ровзинга', NULL UNION
SELECT 12170, 'DASBM', 'DASlog', NULL, 'combobox', 'Симптом Бартомье-Михельсона', NULL UNION
SELECT 12180, 'DASSO', 'DASlog', NULL, 'combobox', 'Симптом Образцова', NULL UNION
SELECT 12190, 'DASV', 'DASlog', NULL, 'combobox', 'Симптом Воскресенского', NULL UNION
SELECT 12200, 'DASC', 'DASlog', NULL, 'combobox', 'Симптом Курвуазье', NULL UNION
SELECT 12210, 'DASMG', 'DASlog', NULL, 'combobox', 'Симптом Мюсси-Георгиевского', NULL UNION
SELECT 12220, 'DASK', 'DASlog', NULL, 'combobox', 'Симптом Кера', NULL UNION
SELECT 12230, 'DASOrtner', 'DASlog', NULL, 'combobox', 'Симптом Ортнера', NULL UNION
SELECT 12240, 'DASTshB', 'DASlog', NULL, 'combobox', 'Симптом Щеткина-Блюмберга', NULL UNION
SELECT 12250, 'DAlogo', 'DASlog', NULL, 'textedit', 'Симптомы (дополнительно)', 'дополнительно' UNION
SELECT 12260, 'DAL', 'DAall', NULL, 'container', 'Печень', NULL UNION
SELECT 12270, 'DALPer', 'DAL', NULL, 'container', 'перкуторно (размеры по методу Курлова)', NULL UNION
SELECT 12280, 'DALPer1t', 'DALPer', NULL, 'textedit', 'по l. mediaclavicularis dextra', NULL UNION
SELECT 12290, 'DALPer2t', 'DALPer', NULL, 'textedit', 'по l. mediasternalis anterior', NULL UNION
SELECT 12300, 'DALPer3t', 'DALPer', NULL, 'textedit', 'по l. costalis sinistra:', NULL UNION
SELECT 12310, 'DALPal', 'DAL', NULL, 'container', 'пальпаторно', NULL UNION
SELECT 12320, 'DALPalC', 'DALPal', NULL, 'textedit', 'Консистенция', NULL UNION
SELECT 12330, 'DALPalT', 'DALPal', NULL, 'textedit', 'Поверхность', NULL UNION
SELECT 12340, 'DALPalP', 'DALPal', NULL, 'textedit', 'Болезненность', NULL UNION
SELECT 12349, 'DALPalE_', 'DALPal', NULL, 'container', 'край', NULL UNION
SELECT 12350, 'DALPalE', 'DALPalE_', NULL, 'combobox', NULL, NULL UNION
SELECT 12360, 'DALPalEd2', 'DALPalE_', NULL, 'combobox', NULL, NULL UNION
SELECT 12370, 'DALPalEd3', 'DALPalE_', NULL, 'combobox', NULL, NULL UNION
SELECT 12380, 'DALBBd', 'DALPalE_', NULL, 'combobox', NULL, NULL UNION
SELECT 12390, 'DALo', 'DAL', NULL, 'textedit', 'Печень (дополнительно)', 'дополнительно' UNION
SELECT 12400, 'DAS', 'DAall', NULL, 'container', 'Селезенка', NULL UNION
SELECT 12410, 'DASPal', 'DAS', NULL, 'textedit', 'пальпаторно', NULL UNION
SELECT 12420, 'DASPer', 'DAS', NULL, 'textedit', 'перкуторно', NULL UNION
SELECT 12430, 'DASo', 'DAS', NULL, 'textedit', 'Селезенка (дополнительно)', 'дополнительно' UNION
SELECT 12440, 'DAP', 'DAall', NULL, 'combobox', 'Перистальтика', NULL UNION
SELECT 12450, 'DAF', 'DAall', NULL, 'combobox', 'Газы', NULL UNION
SELECT 12460, 'DAAR', 'DAall', NULL, 'container', 'Задний проход и прямая кишка', NULL UNION
SELECT 12470, 'DAARA', 'DAAR', NULL, 'combobox', 'При осмотре заднего прохода', NULL UNION
SELECT 12480, 'DAARR', 'DAAR', NULL, 'container', 'При ректальном исследовании', NULL UNION
SELECT 12490, 'DAARRTS', 'DAARR', NULL, 'combobox', 'тонус сфинктера', NULL UNION
SELECT 12500, 'DAARRAm', 'DAARR', NULL, 'combobox', 'ампула прямой кишки', NULL UNION
SELECT 12510, 'DAARRE', 'DAARR', NULL, 'combobox', 'на перчатке', NULL UNION
SELECT 12520, 'DAd', 'DAall', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 12010, 'DAT', 'чистый' UNION
SELECT 12020, 'DAT', 'влажный' UNION
SELECT 12030, 'DAT', 'сухой' UNION
SELECT 12040, 'DAT', 'обложен белым налетом' UNION
SELECT 12050, 'DAT', 'обложен бурым налетом' UNION
SELECT 12060, 'DAT', 'обложен грязным налетом' UNION
SELECT 12070, 'DATT', 'не увеличены' UNION
SELECT 12080, 'DATT', 'увеличены' UNION
SELECT 12090, 'DATT', 'физиологической окраски' UNION
SELECT 12100, 'DATT', 'гиперемированы' UNION
SELECT 12110, 'DATT', 'покрыты белым налетом' UNION
SELECT 12120, 'DAA', 'правильной формы' UNION
SELECT 12130, 'DAA', 'мягкий' UNION
SELECT 12140, 'DAA', 'напряжен' UNION
SELECT 12150, 'DAA', 'не вздут' UNION
SELECT 12160, 'DAA', 'поддут' UNION
SELECT 12170, 'DAA', 'вздут' UNION
SELECT 12180, 'DAA', 'безболезненный во всех отделах' UNION
SELECT 12190, 'DAA', 'болезненный' UNION
SELECT 12200, 'DAA', 'участвует в акте дыхания' UNION
SELECT 12210, 'DAA', 'ограничено участвует в акте дыхания' UNION
SELECT 12220, 'DAA', 'не участвует в акте дыхания' UNION
SELECT 12230, 'DAA', 'напряжения мышц передней брюшной стенки нет' UNION
SELECT 12240, 'DAA', 'определяется умеренное напряжение мышц передней брюшной стенки' UNION
SELECT 12250, 'DAA', 'определяется выраженное напряжение мышц передней брюшной стенки' UNION
SELECT 12260, 'DAAJ', 'пальпаторно без патологии' UNION
SELECT 12270, 'DAAJ', 'не урчит' UNION
SELECT 12280, 'DAAJ', 'слабо урчит' UNION
SELECT 12290, 'DAAJ', 'грубо урчит' UNION
SELECT 12300, 'DAAJ', 'спазмированности нет' UNION
SELECT 12310, 'DAAJ', 'уплотнена' UNION
SELECT 12320, 'DAAJ', 'спазмирована' UNION
SELECT 12330, 'DAAJ', 'выражено спазмирована' UNION
SELECT 12340, 'DAAJ', 'безболезненная' UNION
SELECT 12350, 'DAAJ', 'слабо болезненная' UNION
SELECT 12360, 'DAAJ', 'болезненная' UNION
SELECT 12370, 'DAAJ', 'выражено болезненная' UNION
SELECT 12380, 'DAAC', 'пальпаторно без патологии' UNION
SELECT 12390, 'DAAC', 'не урчит' UNION
SELECT 12400, 'DAAC', 'слабо урчит' UNION
SELECT 12410, 'DAAC', 'грубо урчит' UNION
SELECT 12420, 'DAAC', 'спазмированности нет' UNION
SELECT 12430, 'DAAC', 'уплотнена' UNION
SELECT 12440, 'DAAC', 'спазмирована' UNION
SELECT 12450, 'DAAC', 'выражено спазмирована' UNION
SELECT 12460, 'DAAC', 'безболезненная' UNION
SELECT 12470, 'DAAC', 'слабо болезненная' UNION
SELECT 12480, 'DAAC', 'болезненная' UNION
SELECT 12490, 'DAAC', 'выражено болезненная' UNION
SELECT 12500, 'DAAS', 'пальпаторно без патологии' UNION
SELECT 12510, 'DAAS', 'не урчит' UNION
SELECT 12520, 'DAAS', 'слабо урчит' UNION
SELECT 12530, 'DAAS', 'грубо урчит' UNION
SELECT 12540, 'DAAS', 'спазмированности нет' UNION
SELECT 12550, 'DAAS', 'уплотнена' UNION
SELECT 12560, 'DAAS', 'спазмирована' UNION
SELECT 12570, 'DAAS', 'выражено спазмирована' UNION
SELECT 12580, 'DAAS', 'безболезненная' UNION
SELECT 12590, 'DAAS', 'слабо болезненная' UNION
SELECT 12600, 'DAAS', 'болезненная' UNION
SELECT 12610, 'DAAS', 'выражено болезненная' UNION
SELECT 12620, 'DASVA', 'отрицательный' UNION
SELECT 12630, 'DASVA', 'положительный' UNION
SELECT 12640, 'DASM', 'отрицательный' UNION
SELECT 12650, 'DASM', 'положительный' UNION
SELECT 12660, 'DASR', 'отрицательный' UNION
SELECT 12670, 'DASR', 'положительный' UNION
SELECT 12680, 'DASBM', 'отрицательный' UNION
SELECT 12690, 'DASBM', 'положительный' UNION
SELECT 12700, 'DASSO', 'отрицательный' UNION
SELECT 12710, 'DASSO', 'положительный' UNION
SELECT 12720, 'DASV', 'отрицательный' UNION
SELECT 12730, 'DASV', 'положительный' UNION
SELECT 12740, 'DASC', 'отрицательный' UNION
SELECT 12750, 'DASC', 'положительный' UNION
SELECT 12760, 'DASMG', 'отрицательный' UNION
SELECT 12770, 'DASMG', 'положительный' UNION
SELECT 12780, 'DASK', 'отрицательный' UNION
SELECT 12790, 'DASK', 'положительный' UNION
SELECT 12800, 'DASOrtner', 'отрицательный' UNION
SELECT 12810, 'DASOrtner', 'положительный' UNION
SELECT 12820, 'DASTshB', 'отрицательный' UNION
SELECT 12830, 'DASTshB', 'положительный' UNION
SELECT 12840, 'DALPer', 'печеночная тупость не определяется' UNION
SELECT 12850, 'DALPal', 'край' UNION
SELECT 12860, 'DALPalE', 'острый' UNION
SELECT 12870, 'DALPalE', 'закруглённый' UNION
SELECT 12880, 'DALPalEd2', 'ровный' UNION
SELECT 12890, 'DALPalEd2', 'фестончатый' UNION
SELECT 12900, 'DALPalEd3', 'мягкий' UNION
SELECT 12910, 'DALPalEd3', 'плотный' UNION
SELECT 12920, 'DALBBd', 'располагается по ребёрной дуге' UNION
SELECT 12930, 'DALBBd', 'располагается ниже края ребёрной дуги на 1 см' UNION
SELECT 12940, 'DALBBd', 'располагается ниже края ребёрной дуги на 2 см' UNION
SELECT 12950, 'DALBBd', 'располагается ниже края ребёрной дуги на 3 см' UNION
SELECT 12960, 'DALBBd', 'располагается ниже края ребёрной дуги на 4 см' UNION
SELECT 12970, 'DALBBd', 'располагается ниже края ребёрной дуги на 5 см' UNION
SELECT 12980, 'DAP', 'отчетливая' UNION
SELECT 12990, 'DAP', 'бурная' UNION
SELECT 13000, 'DAP', 'вялая' UNION
SELECT 13010, 'DAP', 'не выслушивается' UNION
SELECT 13020, 'DAF', 'отходят' UNION
SELECT 13030, 'DAF', 'отходят плохо' UNION
SELECT 13040, 'DAF', 'не отходят' UNION
SELECT 13050, 'DAARA', 'патологии не наблюдается' UNION
SELECT 13060, 'DAARR', 'наблюдается выраженная болезненность' UNION
SELECT 13070, 'DAARRTS', 'повышен' UNION
SELECT 13080, 'DAARRTS', 'сохранен' UNION
SELECT 13090, 'DAARRTS', 'снижен' UNION
SELECT 13100, 'DAARRTS', 'отсутствует' UNION
SELECT 13110, 'DAARRAm', 'свободна, проходима' UNION
SELECT 13120, 'DAARRAm', 'содержит каловые массы' UNION
SELECT 13130, 'DAARRE', 'следы кала обычного цвета' UNION
SELECT 13140, 'DAARRE', 'следы кала черного цвета' UNION
SELECT 13150, 'DAARRE', 'следы алой крови' UNION
SELECT 13160, 'DAARRE', 'следы темной крови';


-------------------------
-- Мочеполовая система --
-------------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 14010, 'UGSall', 'main', CAST(NULL AS "numeric"), 'container', 'Мочеполовая система', NULL UNION
SELECT 14020, 'UGSL', 'UGSall', NULL, 'textedit', 'Осмотр поясницы', NULL UNION
SELECT 14030, 'UGSPK', 'UGSall', NULL, 'textedit', 'Пальпация почек', NULL UNION
SELECT 14040, 'UGSSP', 'UGSall', NULL, 'container', 'Cимптом Пастернацкого', NULL UNION
SELECT 14050, 'UGSSPR', 'UGSSP', NULL, 'combobox', 'Справа', NULL UNION
SELECT 14060, 'UGSSPL', 'UGSSP', NULL, 'combobox', 'Слева', NULL UNION
SELECT 14070, 'UGSGS', 'UGSall', NULL, 'textedit', 'Гинекологическое исследование', NULL UNION
SELECT 14080, 'UGSd', 'UGSall', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 14010, 'UGSSPR', 'отрицательный' UNION
SELECT 14020, 'UGSSPR', 'сомнительный' UNION
SELECT 14030, 'UGSSPR', 'положительный' UNION
SELECT 14040, 'UGSSPR', 'резко положительный' UNION
SELECT 14050, 'UGSSPL', 'отрицательный' UNION
SELECT 14060, 'UGSSPL', 'сомнительный' UNION
SELECT 14070, 'UGSSPL', 'положительный' UNION
SELECT 14080, 'UGSSPL', 'резко положительный';


---------------------------------
-- Физиологические отправления --
---------------------------------
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label, shortLabel)
SELECT 15010, 'FECall', 'main', CAST(NULL AS "numeric"), 'container', 'Физиологические отправления', NULL UNION
SELECT 15020, 'FECM', 'FECall', NULL, 'combobox', 'Диурез', NULL UNION
SELECT 15030, 'FECS', 'FECall', NULL, 'combobox', 'Стул', NULL UNION
SELECT 15040, 'FECd', 'FECall', NULL, 'textedit', 'Дополнительно', NULL;

INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 15010, 'FECM', 'не снижен' UNION
SELECT 15020, 'FECM', 'снижен' UNION
SELECT 15030, 'FECM', 'повышен' UNION
SELECT 15040, 'FECM', 'поллакиурия' UNION
SELECT 15050, 'FECM', 'никтурия' UNION
SELECT 15060, 'FECM', 'олигурия' UNION
SELECT 15070, 'FECM', 'полиурия' UNION
SELECT 15080, 'FECM', 'анурия' UNION
SELECT 15090, 'FECS', 'без особенностей' UNION
SELECT 15100, 'FECS', 'для осмотра не предоставлен' UNION
SELECT 15110, 'FECS', 'обильный' UNION
SELECT 15120, 'FECS', 'скудный' UNION
SELECT 15130, 'FECS', 'оформленный' UNION
SELECT 15140, 'FECS', 'неоформленный' UNION
SELECT 15150, 'FECS', 'жидкий' UNION
SELECT 15160, 'FECS', 'слизистый' UNION
SELECT 15170, 'FECS', 'пенистый' UNION
SELECT 15180, 'FECS', 'дегтеобразный' UNION
SELECT 15190, 'FECS', 'с гнилостным запахом' UNION
SELECT 15200, 'FECS', 'без патологических примесей' UNION
SELECT 15210, 'FECS', 'с примесью алой крови';
