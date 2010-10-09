/* This file is part of Clementine.

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ARTISTINFOFETCHER_H
#define ARTISTINFOFETCHER_H

#include <QMap>
#include <QObject>
#include <QUrl>

#include "collapsibleinfopane.h"

class ArtistInfoProvider;

class ArtistInfoFetcher : public QObject {
  Q_OBJECT

public:
  ArtistInfoFetcher(QObject* parent = 0);

  struct Result {
    QList<QUrl> images_;
    QList<CollapsibleInfoPane::Data> info_;
  };

  int FetchInfo(const QString& artist);

signals:
  void ResultReady(int id, const ArtistInfoFetcher::Result& result);

private slots:
  void ImageReady(int id, const QUrl& url);
  void InfoReady(int id, const CollapsibleInfoPane::Data& data);
  void ProviderFinished(int id);

private:
  void AddProvider(ArtistInfoProvider* provider);

private:
  QList<ArtistInfoProvider*> providers_;

  QMap<int, Result> results_;
  QMap<int, QList<ArtistInfoProvider*> > waiting_for_;

  int next_id_;
};

#endif // ARTISTINFOFETCHER_H
