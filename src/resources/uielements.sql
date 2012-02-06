-- Типы элементов.
INSERT INTO UiElementType 
SELECT 'container' 
UNION SELECT 'combobox'
UNION SELECT 'lineedit'
UNION SELECT 'textedit'
UNION SELECT 'spinbox';


INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 01, 'main', NULL, CAST(NULL AS "numeric"), 'container', NULL;


-- Элементы интерфейса.
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 10, 'PA', 'main', CAST(NULL AS "numeric"), 'container', 'Анамнез жизни' UNION
SELECT 20, 'PA_PD', 'PA', NULL, 'lineedit', 'Перенесённые заболевания' UNION
SELECT 30, 'PA_TO', 'PA', NULL, 'lineedit', 'Травмы и операции' UNION
SELECT 40, 'PA_VH', 'PA', NULL, 'lineedit', 'Вирусный гепатит' UNION
SELECT 50, 'PA_Tu', 'PA', NULL, 'lineedit', 'Туберкулёз' UNION
SELECT 60, 'PA_VD', 'PA', NULL, 'lineedit', 'Венерические заболевания' UNION
SELECT 70, 'PA_PM', 'PA', NULL, 'lineedit', 'Сахарный диабет' UNION
SELECT 80, 'PA_EA', 'PA', NULL, 'combobox', 'Эпидемиологический анамнез' UNION
SELECT 90, 'PA_AA', 'PA', NULL, 'lineedit', 'Аллергологический анамнез' UNION
SELECT 100, 'PA_HT', 'PA', NULL, 'lineedit', 'Гемотрансфузии в анамнезе' UNION
SELECT 110, 'PA_UI', 'PA', NULL, 'combobox', 'Привычные интоксикации' UNION
SELECT 120, 'PA_AH', 'PA', NULL, 'lineedit', 'Наследственность' UNION
SELECT 130, 'PA_FS', 'PA', NULL, 'lineedit', 'Семейное положение' UNION
SELECT 140, 'PA_UP', 'PA', NULL, 'combobox', 'Условия проживания' UNION
SELECT 150, 'PA_OGA', 'PA', NULL, 'lineedit', 'Акушерско-гинекологический анамнез' UNION
SELECT 160, 'PA_M', 'PA', NULL, 'lineedit', 'Menses' UNION
SELECT 170, 'PA_exp', 'PA', NULL, 'combobox', 'Страховой анамнез' UNION
SELECT 180, 'PAd', 'PA', NULL, 'lineedit', 'Дополнительно';

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
SELECT 120, 'PA_UP', 'не имеет постоянного места жительства' UNION
SELECT 130, 'PA_exp', 'работающий' UNION
SELECT 140, 'PA_exp', 'неработающий' UNION
SELECT 150, 'PA_exp', 'инвалид III группы' UNION
SELECT 160, 'PA_exp', 'инвалид II группы' UNION
SELECT 170, 'PA_exp', 'инвалид I группы' UNION
SELECT 180, 'PA_exp', 'открытого больничного листа нет' UNION
SELECT 190, 'PA_exp', 'открыт больничный лист № с' UNION
SELECT 200, 'PA_exp', 'КЭК проводилась';



INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 1010, 'USall', 'main', CAST(NULL AS "numeric"), 'container', 'Общий осмотр' UNION
SELECT 1020, 'US_GC', 'USall', NULL, 'combobox', 'Общее состояние' UNION
SELECT 1030, 'US_C', 'USall', NULL, 'combobox', 'Сознание' UNION
SELECT 1040, 'US_PP', 'USall', NULL, 'combobox', 'Положение пациента' UNION
SELECT 1050, 'US_Look', 'USall', NULL, 'combobox', 'Выражение лица' UNION
SELECT 1060, 'US_Gait', 'USall', NULL, 'combobox', 'Походка' UNION
SELECT 1070, 'US_BB', 'USall', NULL, 'combobox', 'Телосложение' UNION
--SELECT 1080, 'US_CT', 'USall', NULL, 'combobox', 'Конституциональный тип' UNION
SELECT 1090, 'US_MB', 'USall', NULL, 'lineedit', 'Масса тела, кг' UNION
SELECT 1100, 'US_Stature', 'USall', NULL, 'lineedit', 'Рост, см' UNION
SELECT 1110, 'US_TB', 'USall', NULL, 'lineedit', 'Температура тела, °C' UNION
SELECT 1120, 'US_Coverlet', 'USall', NULL, 'combobox', 'Кожные покровы' UNION
SELECT 1130, 'US_MV', 'USall', NULL, 'combobox', 'Видимые слизистые' UNION
SELECT 1140, 'US_SF', 'USall', NULL, 'combobox', 'Подкожно-жировая клетчатка' UNION
SELECT 1150, 'US_E', 'USall', NULL, 'lineedit', 'Отеки' UNION
SELECT 1160, 'US_S', 'USall', NULL, 'container', 'Придатки кожи' UNION
--SELECT 1170, 'US_Sd', 'USall', NULL, 'combobox', '' UNION
SELECT 1180, 'US_SHt', 'US_S', NULL, 'lineedit', 'Волосы' UNION
SELECT 1190, 'US_SNt', 'US_S', NULL, 'lineedit', 'Ногти' UNION
SELECT 1200, 'US_SMt', 'US_S', NULL, 'lineedit', 'Молочные железы' UNION
SELECT 1210, 'US_Gd', 'USall', NULL, 'combobox', 'Лимфатические узлы' UNION
SELECT 1220, 'US_G', 'USall', NULL, 'container', 'Лимфатические узлы' UNION
SELECT 1230, 'US_Gsm', 'US_G', NULL, 'combobox', 'подчелюстные' UNION
SELECT 1240, 'US_Gce', 'US_G', NULL, 'combobox', 'шейные' UNION
SELECT 1250, 'US_Gsc', 'US_G', NULL, 'combobox', 'надключичные' UNION
SELECT 1260, 'US_Gbc', 'US_G', NULL, 'combobox', 'подключичные' UNION
SELECT 1270, 'US_Gsa', 'US_G', NULL, 'combobox', 'подмышечные' UNION
SELECT 1280, 'US_Glo', 'US_G', NULL, 'combobox', 'локтевые' UNION
SELECT 1290, 'US_Gpa', 'US_G', NULL, 'combobox', 'паховые' UNION
SELECT 1300, 'US_Gbe', 'US_G', NULL, 'combobox', 'бедренные' UNION
SELECT 1310, 'USd', 'USall', NULL, 'lineedit', 'Дополнительно';

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
--SELECT 1310, 'US_CT', 'астенический' UNION
--SELECT 1320, 'US_CT', 'нормостенический' UNION
--SELECT 1330, 'US_CT', 'гиперстенический' UNION
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
--SELECT 1610, 'US_Sd', 'Придатки кожи без особенностей' UNION
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



INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 3010, 'MSSall', 'main', CAST(NULL AS "numeric"), 'container', 'Опорно-двигательный аппарат' UNION
SELECT 3020, 'MSSS', 'MSSall', NULL, 'lineedit', 'Костные структуры' UNION
SELECT 3030, 'MSSM', 'MSSall', NULL, 'container', 'Мышечная система' UNION
SELECT 3040, 'NS_dva', 'MSSM', NULL, 'lineedit', 'Атрофия мышц' UNION
SELECT 3050, 'NS_dvg', 'MSSM', NULL, 'lineedit', 'Истинная гипертрофия мышц' UNION
SELECT 3060, 'NS_dvp', 'MSSM', NULL, 'lineedit', 'Псевдогипертрофии' UNION
SELECT 3070, 'NS_dvf', 'MSSM', NULL, 'lineedit', 'Фибриллярные и фасцикулярные' UNION
SELECT 3080, 'NS_dvr', 'MSSM', NULL, 'lineedit', 'Ретракции и контрактуры' UNION
SELECT 3090, 'NS_dvo', 'MSSM', NULL, 'lineedit', 'Объем движений в конечностях' UNION
SELECT 3100, 'NS_dvs', 'MSSM', NULL, 'lineedit', 'Сила мышц дистальных и' UNION
SELECT 3110, 'NS_dvt', 'MSSM', NULL, 'lineedit', 'Тонус мышц' UNION
SELECT 3120, 'MSSJ', 'MSSall', NULL, 'container', 'Суставы' UNION
SELECT 3130, 'MSSJ_1', 'MSSJ', NULL, 'lineedit', 'Конфигурация' UNION
SELECT 3140, 'MSSJ_2', 'MSSJ', NULL, 'lineedit', 'Болезненность при пальпации' UNION
SELECT 3150, 'MSSJ_3', 'MSSJ', NULL, 'lineedit', 'Активные движения' UNION
SELECT 3160, 'MSSJ_4', 'MSSJ', NULL, 'lineedit', 'Пассивные движения' UNION
SELECT 3170, 'MSSd', 'MSSall', NULL, 'lineedit', 'Дополнительно';

--INSERT INTO UiElementEnums(id, uiElementTextId, value)
--SELECT 3010, 'MSSM', 'без явных признаков патологии' UNION
--SELECT 3020, 'MSSJ', 'без явных признаков патологии';



INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 4010, 'Neckall', 'main', CAST(NULL AS "numeric"), 'container', 'Шея' UNION
SELECT 4020, 'NeckF', 'Neckall', NULL, 'lineedit', 'Форма шеи' UNION
SELECT 4030, 'NeckV', 'Neckall', NULL, 'combobox', 'Голос' UNION
SELECT 4040, 'NeckTG', 'Neckall', NULL, 'container', 'Щитовидная железа' UNION
SELECT 4050, 'NeckTGS', 'NeckTG', NULL, 'combobox', 'Размеры' UNION
SELECT 4060, 'NeckTGM', 'NeckTG', NULL, 'combobox', 'Подвижность' UNION
SELECT 4070, 'NeckTGP', 'NeckTG', NULL, 'combobox', 'Болезненность при пальпации' UNION
SELECT 4080, 'NeckTGCS', 'NeckTG', NULL, 'combobox', 'Консистенция' UNION
SELECT 4090, 'NeckTGSU', 'NeckTG', NULL, 'combobox', 'Поверхность' UNION
SELECT 4100, 'NeckD', 'Neckall', NULL, 'lineedit', 'Дополнительно';

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



INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 5010, 'OVall', 'main', CAST(NULL AS "numeric"), 'container', 'Органы зрения' UNION
SELECT 5020, 'NS_cnzo', 'OVall', NULL, 'lineedit', 'Острота зрения' UNION
SELECT 5030, 'NS_cnzc', 'OVall', NULL, 'lineedit', 'Цветоощущение' UNION
SELECT 5040, 'NS_cnzp', 'OVall', NULL, 'lineedit', 'Поля зрения' UNION
SELECT 5050, 'NS_cngd', 'OVall', NULL, 'lineedit', 'Диплопия' UNION
SELECT 5060, 'US_OVemot', 'OVall', NULL, 'lineedit', 'Движения глазных яблок' UNION
SELECT 5070, 'US_OVMio', 'OVall', NULL, 'combobox', 'Миоз' UNION
SELECT 5080, 'US_OVMyd', 'OVall', NULL, 'combobox', 'Мидриаз' UNION
SELECT 5090, 'US_OVEno', 'OVall', NULL, 'combobox', 'Энофтальм' UNION
SELECT 5100, 'US_OVExo', 'OVall', NULL, 'combobox', 'Экзофтальм' UNION
SELECT 5110, 'US_OVCS', 'OVall', NULL, 'combobox', 'Цвет склер' UNION
SELECT 5120, 'US_OVd', 'OVall', NULL, 'lineedit', 'Дополнительно';

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


INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 6010, 'RAall', 'main', CAST(NULL AS "numeric"), 'container', 'Органы дыхания' UNION
SELECT 6020, 'US_RAFT', 'RAall', NULL, 'combobox', 'Форма грудной клетки' UNION
SELECT 6030, 'US_RAFRM', 'RAall', NULL, 'lineedit', 'Частота дыхательных движений' UNION
SELECT 6040, 'US_RACB', 'RAall', NULL, 'combobox', 'Одышка' UNION
SELECT 6050, 'US_RAPR', 'RAall', NULL, 'combobox', 'Патологическое дыхание' UNION
SELECT 6060, 'US_RAP', 'RAall', NULL, 'lineedit', 'Пальпация грудной клетки' UNION
SELECT 6070, 'US_RAPS', 'RAall', NULL, 'lineedit', 'Перкуторный звук' UNION
SELECT 6080, 'US_RAA', 'RAall', NULL, 'combobox', 'Аускультативно' UNION
SELECT 6090, 'US_RAW', 'RAall', NULL, 'lineedit', 'Хрипы' UNION
SELECT 6100, 'US_RAPFR', 'RAall', NULL, 'combobox', 'Шум трения плевры' UNION
SELECT 6110, 'US_RAVT', 'RAall', NULL, 'combobox', 'Голосовое дрожание' UNION
SELECT 6120, 'US_RAd', 'RAall', NULL, 'lineedit', 'Дополнительно';

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





INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 7010, 'CVSall', 'main', CAST(NULL AS "numeric"), 'container', 'Сердечно-сосудистая система' UNION
SELECT 7020, 'CVSPH', 'CVSall', NULL, 'combobox', 'Перкуссия сердца' UNION
SELECT 7030, 'CVSAH', 'CVSall', NULL, 'combobox', 'Аускультация сердца' UNION
SELECT 7040, 'CVSHB', 'CVSall', NULL, 'combobox', 'Сердечные сокращения' UNION
SELECT 7045, 'CSSHB_', 'CVSall', NULL, 'lineedit', 'Частота сердечных сокращений' UNION
SELECT 7050, 'CVSP', 'CVSall', NULL, 'combobox', 'Пульс' UNION
SELECT 7055, 'CVSP_', 'CVSall', NULL, 'combobox', 'Частота пульса' UNION
SELECT 7060, 'CVSJS', 'CVSall', NULL, 'lineedit', 'Дефицит пульса' UNION
SELECT 7070, 'CVSAP', 'CVSall', NULL, 'container', 'Артериальное давление' UNION
SELECT 7080, 'CVSAPRTt', 'CVSAP', NULL, 'lineedit', 'на правой верхней конечности' UNION
SELECT 7090, 'CVSAPLTt', 'CVSAP', NULL, 'lineedit', 'на левой верхней конечности' UNION
SELECT 7100, 'CVSAPRBt', 'CVSAP', NULL, 'lineedit', 'на правой нижней конечности' UNION
SELECT 7110, 'CVSAPLBt', 'CVSAP', NULL, 'lineedit', 'на левой нижней конечности' UNION
SELECT 7120, 'CVSAT', 'CVSall', NULL, 'combobox', 'Верхушечный толчок' UNION
SELECT 7130, 'CVSHT', 'CVSall', NULL, 'lineedit', 'Сердечный толчок' UNION
SELECT 7140, 'CVSPM', 'CVSall', NULL, 'combobox', 'Шум трения перикарда' UNION
SELECT 7150, 'CVSCCAR', 'CVSall', NULL, 'container', 'Правая общая сонная артерия' UNION
SELECT 7160, 'CVSCCARP', 'CVSCCAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7170, 'CVSCCARN', 'CVSCCAR', NULL, 'combobox', 'шумы' UNION
SELECT 7180, 'CVSCCARo', 'CVSCCAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7190, 'CVSCCAL', 'CVSall', NULL, 'container', 'Левая общая сонная артерия' UNION
SELECT 7200, 'CVSCCALP', 'CVSCCAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7210, 'CVSCCALN', 'CVSCCAL', NULL, 'combobox', 'шумы' UNION
SELECT 7220, 'CVSCCALo', 'CVSCCAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7230, 'CVSCAR', 'CVSall', NULL, 'container', 'Правая подключичная артерия' UNION
SELECT 7240, 'CVSCARN', 'CVSCAR', NULL, 'combobox', 'шумы' UNION
SELECT 7250, 'CVSCARo', 'CVSCAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7260, 'CVSCAL', 'CVSall', NULL, 'container', 'Левая подключичная артерия' UNION
SELECT 7270, 'CVSCALN', 'CVSCAL', NULL, 'combobox', 'шумы' UNION
SELECT 7280, 'CVSCALo', 'CVSCAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7290, 'CVSBAR', 'CVSall', NULL, 'container', 'Правая плечевая артерия' UNION
SELECT 7300, 'CVSBARP', 'CVSBAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7310, 'CVSBARN', 'CVSBAR', NULL, 'combobox', 'шумы' UNION
SELECT 7320, 'CVSBARo', 'CVSBAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7330, 'CVSBAL', 'CVSall', NULL, 'container', 'Левая плечевая артерия' UNION
SELECT 7340, 'CVSBALP', 'CVSBAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7350, 'CVSBALN', 'CVSBAL', NULL, 'combobox', 'шумы' UNION
SELECT 7360, 'CVSBALo', 'CVSBAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7370, 'CVSRAR', 'CVSall', NULL, 'container', 'Правая лучевая артерия' UNION
SELECT 7380, 'CVSRARP', 'CVSRAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7390, 'CVSRARo', 'CVSRAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7400, 'CVSRAL', 'CVSall', NULL, 'container', 'Левая лучевая артерия' UNION
SELECT 7410, 'CVSRALP', 'CVSRAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7420, 'CVSRALo', 'CVSRAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7430, 'CVSAA', 'CVSall', NULL, 'container', 'Брюшная аорта' UNION
SELECT 7440, 'CVSAAP', 'CVSAA', NULL, 'combobox', 'пульсация' UNION
SELECT 7450, 'CVSAAN', 'CVSAA', NULL, 'combobox', 'шумы' UNION
SELECT 7460, 'CVSAAo', 'CVSAA', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7470, 'CVSFAR', 'CVSall', NULL, 'container', 'Правая общая бедренная артерия' UNION
SELECT 7480, 'CVSFARP', 'CVSFAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7490, 'CVSFARN', 'CVSFAR', NULL, 'combobox', 'шумы' UNION
SELECT 7500, 'CVSFARo', 'CVSFAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7510, 'CVSFAL', 'CVSall', NULL, 'container', 'Левая общая бедренная артерия' UNION
SELECT 7520, 'CVSFALP', 'CVSFAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7530, 'CVSFALN', 'CVSFAL', NULL, 'combobox', 'шумы' UNION
SELECT 7540, 'CVSFALo', 'CVSFAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7550, 'CVSPAR', 'CVSall', NULL, 'container', 'Правая подколенная артерия' UNION
SELECT 7560, 'CVSPARP', 'CVSPAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7570, 'CVSPARN', 'CVSPAR', NULL, 'combobox', 'шумы' UNION
SELECT 7580, 'CVSPARo', 'CVSPAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7590, 'CVSPAL', 'CVSall', NULL, 'container', 'Левая подколенная артерия' UNION
SELECT 7600, 'CVSPALP', 'CVSPAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7610, 'CVSPALN', 'CVSPAL', NULL, 'combobox', 'шумы' UNION
SELECT 7620, 'CVSPALo', 'CVSPAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7630, 'CVSRFAR', 'CVSall', NULL, 'container', 'Правая артерия тыла стопы' UNION
SELECT 7640, 'CVSRFARP', 'CVSRFAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7650, 'CVSRFARo', 'CVSRFAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7660, 'CVSRFAL', 'CVSall', NULL, 'container', 'Левая артерия тыла стопы' UNION
SELECT 7670, 'CVSRFALP', 'CVSRFAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7680, 'CVSRFALo', 'CVSRFAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7690, 'CVSPTAR', 'CVSall', NULL, 'container', 'Правая задняя большеберцовая артерия' UNION
SELECT 7700, 'CVSPTARP', 'CVSPTAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7710, 'CVSPTARo', 'CVSPTAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7720, 'CVSPTAL', 'CVSall', NULL, 'container', 'Левая задняя большеберцовая артерия' UNION
SELECT 7730, 'CVSPTALP', 'CVSPTAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7740, 'CVSPTALo', 'CVSPTAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7750, 'CVSSVT', 'CVSall', NULL, 'lineedit', 'Подкожные вены верхних конечностей' UNION
SELECT 7760, 'CVSSVB', 'CVSall', NULL, 'lineedit', 'Подкожные вены нижних конечностей' UNION
SELECT 7770, 'CVSCE', 'CVSall', NULL, 'lineedit', 'Окружность конечностей' UNION
SELECT 7780, 'CVSCEd1', 'CVSCE', NULL, 'lineedit', 'правой верхней в средней трети плеча' UNION
SELECT 7790, 'CVSCEd2', 'CVSCE', NULL, 'lineedit', 'левой верхней в средней трети плеча' UNION
SELECT 7800, 'CVSCEd3', 'CVSCE', NULL, 'lineedit', 'правой верхней в средней трети предплечья' UNION
SELECT 7810, 'CVSCEd4', 'CVSCE', NULL, 'lineedit', 'левой верхней в средней трети предплечья' UNION
SELECT 7820, 'CVSCEd5', 'CVSCE', NULL, 'lineedit', 'правой нижней в средней трети бедра' UNION
SELECT 7830, 'CVSCEd6', 'CVSCE', NULL, 'lineedit', 'левой нижней в средней трети бедра' UNION
SELECT 7840, 'CVSCEd7', 'CVSCE', NULL, 'lineedit', 'правой нижней в средней трети голени' UNION
SELECT 7850, 'CVSCEd8', 'CVSCE', NULL, 'lineedit', 'левой нижней в средней трети голени' UNION
SELECT 7860, 'CVSd', 'CVSall', NULL, 'lineedit', 'Дополнительно';

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



INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 7010, 'CVSall', 'main', CAST(NULL AS "numeric"), 'container', 'Сердечно-сосудистая система' UNION
SELECT 7020, 'CVSPH', 'CVSall', NULL, 'combobox', 'Перкуссия сердца' UNION
SELECT 7030, 'CVSAH', 'CVSall', NULL, 'combobox', 'Аускультация сердца' UNION
SELECT 7040, 'CVSHB', 'CVSall', NULL, 'combobox', 'Сердечные сокращения' UNION
SELECT 7045, 'CSSHB_', 'CVSall', NULL, 'lineedit', 'Частота сердечных сокращений' UNION
SELECT 7050, 'CVSP', 'CVSall', NULL, 'combobox', 'Пульс' UNION
SELECT 7050, 'CVSP_', 'CVSall', NULL, 'combobox', 'Частота пульса' UNION
SELECT 7060, 'CVSJS', 'CVSall', NULL, 'lineedit', 'Дефицит пульса' UNION
SELECT 7070, 'CVSAP', 'CVSall', NULL, 'container', 'Артериальное давление' UNION
SELECT 7080, 'CVSAPRTt', 'CVSAP', NULL, 'lineedit', 'на правой верхней конечности' UNION
SELECT 7090, 'CVSAPLTt', 'CVSAP', NULL, 'lineedit', 'на левой верхней конечности' UNION
SELECT 7100, 'CVSAPRBt', 'CVSAP', NULL, 'lineedit', 'на правой нижней конечности' UNION
SELECT 7110, 'CVSAPLBt', 'CVSAP', NULL, 'lineedit', 'на левой нижней конечности' UNION
SELECT 7120, 'CVSAT', 'CVSall', NULL, 'combobox', 'Верхушечный толчок' UNION
SELECT 7130, 'CVSHT', 'CVSall', NULL, 'lineedit', 'Сердечный толчок' UNION
SELECT 7140, 'CVSPM', 'CVSall', NULL, 'combobox', 'Шум трения перикарда' UNION
SELECT 7150, 'CVSCCAR', 'CVSall', NULL, 'container', 'Правая общая сонная артерия' UNION
SELECT 7160, 'CVSCCARP', 'CVSCCAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7170, 'CVSCCARN', 'CVSCCAR', NULL, 'combobox', 'шумы' UNION
SELECT 7180, 'CVSCCARo', 'CVSall', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7190, 'CVSCCAL', 'CVSall', NULL, 'container', 'Левая общая сонная артерия' UNION
SELECT 7200, 'CVSCCALP', 'CVSCCAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7210, 'CVSCCALN', 'CVSCCAL', NULL, 'combobox', 'шумы' UNION
SELECT 7220, 'CVSCCALo', 'CVSCCAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7230, 'CVSCAR', 'CVSall', NULL, 'container', 'Правая подключичная артерия' UNION
SELECT 7240, 'CVSCARN', 'CVSCAR', NULL, 'combobox', 'шумы' UNION
SELECT 7250, 'CVSCARo', 'CVSCAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7260, 'CVSCAL', 'CVSall', NULL, 'container', 'Левая подключичная артерия' UNION
SELECT 7270, 'CVSCALN', 'CVSCAL', NULL, 'combobox', 'шумы' UNION
SELECT 7280, 'CVSCALo', 'CVSCAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7290, 'CVSBAR', 'CVSall', NULL, 'container', 'Правая плечевая артерия' UNION
SELECT 7300, 'CVSBARP', 'CVSBAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7310, 'CVSBARN', 'CVSBAR', NULL, 'combobox', 'шумы' UNION
SELECT 7320, 'CVSBARo', 'CVSBAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7330, 'CVSBAL', 'CVSall', NULL, 'container', 'Левая плечевая артерия' UNION
SELECT 7340, 'CVSBALP', 'CVSBAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7350, 'CVSBALN', 'CVSBAL', NULL, 'combobox', 'шумы' UNION
SELECT 7360, 'CVSBALo', 'CVSBAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7370, 'CVSRAR', 'CVSall', NULL, 'container', 'Правая лучевая артерия' UNION
SELECT 7380, 'CVSRARP', 'CVSRAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7390, 'CVSRARo', 'CVSRAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7400, 'CVSRAL', 'CVSall', NULL, 'container', 'Левая лучевая артерия' UNION
SELECT 7410, 'CVSRALP', 'CVSRAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7420, 'CVSRALo', 'CVSRAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7430, 'CVSAA', 'CVSall', NULL, 'container', 'Брюшная аорта' UNION
SELECT 7440, 'CVSAAP', 'CVSAA', NULL, 'combobox', 'пульсация' UNION
SELECT 7450, 'CVSAAN', 'CVSAA', NULL, 'combobox', 'шумы' UNION
SELECT 7460, 'CVSAAo', 'CVSAA', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7470, 'CVSFAR', 'CVSall', NULL, 'container', 'Правая общая бедренная артерия' UNION
SELECT 7480, 'CVSFARP', 'CVSFAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7490, 'CVSFARN', 'CVSFAR', NULL, 'combobox', 'шумы' UNION
SELECT 7500, 'CVSFARo', 'CVSFAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7510, 'CVSFAL', 'CVSall', NULL, 'container', 'Левая общая бедренная артерия' UNION
SELECT 7520, 'CVSFALP', 'CVSFAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7530, 'CVSFALN', 'CVSFAL', NULL, 'combobox', 'шумы' UNION
SELECT 7540, 'CVSFALo', 'CVSFAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7550, 'CVSPAR', 'CVSall', NULL, 'container', 'Правая подколенная артерия' UNION
SELECT 7560, 'CVSPARP', 'CVSPAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7570, 'CVSPARN', 'CVSPAR', NULL, 'combobox', 'шумы' UNION
SELECT 7580, 'CVSPARo', 'CVSPAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7590, 'CVSPAL', 'CVSall', NULL, 'container', 'Левая подколенная артерия' UNION
SELECT 7600, 'CVSPALP', 'CVSPAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7610, 'CVSPALN', 'CVSPAL', NULL, 'combobox', 'шумы' UNION
SELECT 7620, 'CVSPALo', 'CVSPAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7630, 'CVSRFAR', 'CVSall', NULL, 'container', 'Правая артерия тыла стопы' UNION
SELECT 7640, 'CVSRFARP', 'CVSRFAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7650, 'CVSRFARo', 'CVSRFAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7660, 'CVSRFAL', 'CVSall', NULL, 'container', 'Левая артерия тыла стопы' UNION
SELECT 7670, 'CVSRFALP', 'CVSRFAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7680, 'CVSRFALo', 'CVSRFAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7690, 'CVSPTAR', 'CVSall', NULL, 'container', 'Правая задняя большеберцовая артерия' UNION
SELECT 7700, 'CVSPTARP', 'CVSPTAR', NULL, 'combobox', 'пульсация' UNION
SELECT 7710, 'CVSPTARo', 'CVSPTAR', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7720, 'CVSPTAL', 'CVSall', NULL, 'container', 'Левая задняя большеберцовая артерия' UNION
SELECT 7730, 'CVSPTALP', 'CVSPTAL', NULL, 'combobox', 'пульсация' UNION
SELECT 7740, 'CVSPTALo', 'CVSPTAL', NULL, 'lineedit', 'дополнительно' UNION
SELECT 7750, 'CVSSVT', 'CVSall', NULL, 'lineedit', 'Подкожные вены верхних конечностей' UNION
SELECT 7760, 'CVSSVB', 'CVSall', NULL, 'lineedit', 'Подкожные вены нижних конечностей' UNION
SELECT 7770, 'CVSCE', 'CVSall', NULL, 'lineedit', 'Окружность конечностей' UNION
SELECT 7780, 'CVSCEd1', 'CVSCE', NULL, 'lineedit', 'правой верхней в средней трети плеча' UNION
SELECT 7790, 'CVSCEd2', 'CVSCE', NULL, 'lineedit', 'левой верхней в средней трети плеча' UNION
SELECT 7800, 'CVSCEd3', 'CVSCE', NULL, 'lineedit', 'правой верхней в средней трети предплечья' UNION
SELECT 7810, 'CVSCEd4', 'CVSCE', NULL, 'lineedit', 'левой верхней в средней трети предплечья' UNION
SELECT 7820, 'CVSCEd5', 'CVSCE', NULL, 'lineedit', 'правой нижней в средней трети бедра' UNION
SELECT 7830, 'CVSCEd6', 'CVSCE', NULL, 'lineedit', 'левой нижней в средней трети бедра' UNION
SELECT 7840, 'CVSCEd7', 'CVSCE', NULL, 'lineedit', 'правой нижней в средней трети голени' UNION
SELECT 7850, 'CVSCEd8', 'CVSCE', NULL, 'lineedit', 'левой нижней в средней трети голени' UNION
SELECT 7860, 'CVSd', 'CVSall', NULL, 'lineedit', 'Дополнительно';

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