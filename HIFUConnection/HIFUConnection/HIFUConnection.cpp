#include "HIFUConnection.h"
#include <qdebug.h>

#include "maindialog.h"

HIFUConnection::HIFUConnection():_pMdlg(NULL)
{
    _pMdlg = new MainDialog;

}

HIFUConnection::~HIFUConnection()
{
    if (_pMdlg!=NULL)
    {
        delete _pMdlg;
        _pMdlg = NULL;
    }
}

void HIFUConnection::showWindow()
{
    if(_pMdlg->isHidden())
        _pMdlg->show();
    else
        _pMdlg->activateWindow();

}


void HIFUConnection::ClearData()
{
    //_pMdlg->ClearAllData();
}

void HIFUConnection::on_Maindialog_Close()
{

}

QWidget* HIFUConnection::GetWindowWidget()
{
    return _pMdlg;
}
