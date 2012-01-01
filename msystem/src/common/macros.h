

#pragma once


/// Проверяет, выполнился ли запрос \a query. Если нет, выводит в консоль сообщение
/// об ошибке. Должен быть подключен заголовочный файл <QDebug>
#define checkQuery(query) \
if(!query.isActive()) \
	qWarning() <<	"In file" << __FILE__ << \
					", line" << __LINE__ << \
					", occured error:\n" << query.lastError() << \
					"\nExecuted query:\n" << query.lastQuery() << \
					"\nBound values:\n" << query.boundValues() << "\n";

/// Возвращает строку \a string, если она не является пустой,
/// иначе возвращает QString::null.
#define nullIfEmpty(string) (string.isEmpty() ? QString::null : string)
