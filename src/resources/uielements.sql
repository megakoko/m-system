-- Типы элементов.
INSERT INTO UiElementType 
SELECT 'container' 
UNION SELECT 'combobox'
UNION SELECT 'lineedit'
UNION SELECT 'textedit'
UNION SELECT 'spinbox';


-- Элементы интерфейса.
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 10, 'top1', NULL, CAST(NULL AS "numeric"), 'container', 'top item 1' UNION
SELECT 20, 'combo1', 'top1', NULL, 'combobox', 'combo box 1' UNION
SELECT 30, 'combo2', 'top1', NULL, 'combobox', 'combo box 2' UNION 
SELECT 40, 'line1', 'top1', NULL, 'lineedit', 'line edit 1' UNION
SELECT 50, 'top2', NULL, NULL, 'container', 'top container 2' UNION
SELECT 60, 'combo3', 'top2', NULL, 'combobox', 'combo box 3 inside container 2';

