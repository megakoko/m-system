

#pragma once

#define checkQuery(query) \
if(!query.isActive()) \
	qWarning() << "In line" << __LINE__ << "occured error:\n" << query.lastError() << \
				  "\nExecuted query:\n" << query.lastQuery() << \
				  "\nBound values:\n" << query.boundValues() << "\n";
