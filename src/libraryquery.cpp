#include "libraryquery.h"

#include <QtDebug>
#include <QDateTime>

LibraryQuery::LibraryQuery()
{
}

LibraryQuery::LibraryQuery(const QueryOptions& options)
{
  if (!options.filter.isEmpty()) {
    where_clauses_ << "("
        "artist LIKE ? OR "
        "album LIKE ? OR "
        "title LIKE ?)";
    bound_values_ << "%" + options.filter + "%";
    bound_values_ << "%" + options.filter + "%";
    bound_values_ << "%" + options.filter + "%";
  }

  if (options.max_age != -1) {
    int cutoff = QDateTime::currentDateTime().toTime_t() - options.max_age;

    where_clauses_ << "ctime > ?";
    bound_values_ << cutoff;
  }
}

void LibraryQuery::AddWhere(const QString& column, const QVariant& value) {
  // Do integers inline - sqlite seems to get confused when you pass integers
  // to bound parameters

  if (value.type() == QVariant::Int)
    where_clauses_ << QString("%1 = %2").arg(column, value.toString());
  else {
    where_clauses_ << QString("%1 = ?").arg(column);
    bound_values_ << value;
  }
}

QSqlQuery LibraryQuery::Query(QSqlDatabase db) const {
  QString sql = QString("SELECT %1 FROM songs").arg(column_spec_);

  if (!where_clauses_.isEmpty())
    sql += " WHERE " + where_clauses_.join(" AND ");

  QSqlQuery q(sql, db);

  // Bind values
  foreach (const QVariant& value, bound_values_) {
    q.addBindValue(value);
  }

  return q;
}
