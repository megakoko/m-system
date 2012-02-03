-- Типы элементов.
INSERT INTO UiElementType 
SELECT 'container' 
UNION SELECT 'combobox'
UNION SELECT 'lineedit'
UNION SELECT 'textedit'
UNION SELECT 'spinbox';


-- Элементы интерфейса.
INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)
SELECT 9,  'main', NULL, CAST(NULL AS "numeric"), 'container', 'asd' UNION
SELECT 10, 'top1', 'main', NULL, 'container', 'top item 1' UNION
SELECT 20, 'combo1', 'top1', NULL, 'combobox', 'combo box 1' UNION
SELECT 30, 'combo2', 'top1', NULL, 'combobox', 'combo box 2' UNION 
SELECT 40, 'line1', 'top1', NULL, 'lineedit', 'line edit 1' UNION
SELECT 50, 'top2', 'main', NULL, 'container', 'top container 2' UNION
SELECT 60, 'combo3', 'top2', NULL, 'combobox', 'combo box 3 inside container 2';


INSERT INTO UiElementEnums(id, uiElementTextId, value)
SELECT 1, 'combo1', 'AQ40I4DL91ZGNH5JEHSM' UNION
SELECT 2, 'combo1', 'KQZ36BEKAVP8O71BUGFD' UNION
SELECT 3, 'combo1', 'M1QBNWE37F52SGKP3H0Q' UNION
SELECT 4, 'combo1', 'BD59HWUR1ZQGNM4MA7VM' UNION
SELECT 5, 'combo2', 'TGWGJ1ZRL56F10WMC3O2' UNION
SELECT 6, 'combo2', 'SS83NNXVCOD092IM9BLP' UNION
SELECT 7, 'combo2', 'H9TZB6NFAXOJQWD51JF6' UNION
SELECT 8, 'combo2', 'J92ITAG0AXQZZ5VFLJPO' UNION
SELECT 9, 'combo2', 'IA1ELV1BMV74NLR9L8Z3' UNION
SELECT 10, 'combo2', 'MU21EKL5SCCBVK36VZAU' UNION
SELECT 11, 'combo2', '5QQVV1YEE2FITFFLBXPM' UNION
SELECT 12, 'combo2', 'H7FHNOQWYNZUSWA31HNT' UNION
SELECT 13, 'combo3', '7PCGFLKGBRXDCH0H3Q4A' UNION
SELECT 14, 'combo3', 'S7GKP31QQV2M3C2IGZLV' UNION
SELECT 15, 'combo3', 'D37FS0BN63XR9ZG3X40U' UNION
SELECT 16, 'combo3', 'GD3HJYHEFTUFT26FA6OQ' UNION
SELECT 17, 'combo3', '2HE81SP1UVOHY7L3Q9LK' UNION
SELECT 18, 'combo3', 'LROA62F9R2RJ4FNUVEZ3' UNION
SELECT 19, 'combo3', 'ZEKMZBKZ8JUHONW12YKZ' UNION
SELECT 20, 'combo3', 'UKHGWKSWBHJEWYK8Q4YG' UNION
SELECT 21, 'combo3', 'BZ2V46JK8NLBH6SW8XIB';


