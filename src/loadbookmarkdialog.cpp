/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Micha³ Mêciñski
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "loadbookmarkdialog.h"

#include <QPushButton>

#include "fraqtiveapplication.h"
#include "configurationdata.h"
#include "fractalpresenter.h"
#include "fractalmodel.h"
#include "imageview.h"
#include "iconloader.h"

LoadBookmarkDialog::LoadBookmarkDialog( QWidget* parent ) : QDialog( parent ),
    m_model( NULL )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "load-bookmark", 22 ) );
    m_ui.promptLabel->setText( tr( "Load fractal parameters from a bookmark:" ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    m_ui.listView->setMap( fraqtive()->configuration()->bookmarks() );

    connect( m_ui.listView->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
        this, SLOT( selectionChanged() ) );
    connect( m_ui.listView->model(), SIGNAL( modelReset() ), this, SLOT( selectionChanged() ) );

    m_presenter = new FractalPresenter( this );

    ImageView* view = new ImageView( m_ui.viewContainer, m_presenter );
    m_ui.viewContainer->setView( view );

    m_presenter->setView( view );
    m_presenter->setPriority( 2 );

    selectionChanged();
}

LoadBookmarkDialog::~LoadBookmarkDialog()
{
}

void LoadBookmarkDialog::setModel( FractalModel* model )
{
    m_model = model;

    m_ui.listView->setColorSettings( model->gradient(), model->backgroundColor(), model->colorMapping() );

    m_presenter->setColorSettings( model->gradient(), model->backgroundColor(), model->colorMapping() );
    m_presenter->setGeneratorSettings( model->generatorSettings() );
    m_presenter->setViewSettings( model->viewSettings() );
}

void LoadBookmarkDialog::on_listView_doubleClicked()
{
    accept();
}

void LoadBookmarkDialog::selectionChanged()
{
    QModelIndexList selection = m_ui.listView->selectionModel()->selectedIndexes();
    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setEnabled( selection.count() == 1 );

    if ( selection.count() == 1 ) {
        QString name = m_ui.listView->model()->data( selection[ 0 ] ).toString();
        BookmarkMap* map = fraqtive()->configuration()->bookmarks();
        Bookmark bookmark = map->value( name );
        m_presenter->setParameters( bookmark.fractalType(), bookmark.position() );

        m_presenter->setEnabled( true );
    } else {
        m_presenter->setEnabled( false );
    }
}

void LoadBookmarkDialog::accept()
{
    QModelIndexList selection = m_ui.listView->selectionModel()->selectedIndexes();

    if ( selection.count() == 1 ) {
        QString name = m_ui.listView->model()->data( selection[ 0 ] ).toString();
        BookmarkMap* map = fraqtive()->configuration()->bookmarks();
        Bookmark bookmark = map->value( name );
        m_model->setParameters( bookmark.fractalType(), bookmark.position() );

        QDialog::accept();
    }
}
