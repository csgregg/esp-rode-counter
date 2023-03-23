
// Are you sure? Action
var sureAction;


function initPage() {
    doRequest("","");
}

function updatePage() {
}



function SureNo() {
    document.getElementById('sure_dlg').style.display='none';
    if( sureAction == "log_save" ) disableElmt("log_save","log_cancel");
    doRequestAll();
 }



function SureYes() {

    clearSure();


    if( sureAction == "btn_restart" ) {
        showLoader(document.getElementById('device_box'));
        doRequest(sureAction,1,reloadPage);
    }
    else {
        // All the settings resets
        showLoader(document.getElementById('reset_box'));
        doRequest(sureAction,1,reloadPage);
    }
}


