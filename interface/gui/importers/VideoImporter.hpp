/*
	This file is part of Overmix.

	Overmix is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Overmix is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Overmix.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VIDEO_IMPORTER_HPP
#define VIDEO_IMPORTER_HPP

#include "VideoPreviewModel.hpp"
#include <QDialog>

class Ui_Dialog;

namespace Overmix{
	
class ImageContainer;
class AProcessWatcher;

class VideoImporter : public QDialog{
	Q_OBJECT
	
	private:
		Ui_Dialog *ui;
		QString filepath;
		VideoPreviewModel model;
		
		
	public:
		VideoImporter( QString filepath, QWidget* parent );
		
		static bool supportedFile( QString filename );
		void import( ImageContainer &files, AProcessWatcher* watcher=nullptr );
		
	public slots:
		void refresh();
};

}

#endif

