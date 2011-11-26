

#pragma once

#define checkQuery(query) \
if(!query.isActive()) \
	qWarning() <<	"In file" << __FILE__ << \
					", line" << __LINE__ << \
					", occured error:\n" << query.lastError() << \
					"\nExecuted query:\n" << query.lastQuery() << \
					"\nBound values:\n" << query.boundValues() << "\n";

#define nullIfEmpty(string) (string.isEmpty() ? QString::null : string)
