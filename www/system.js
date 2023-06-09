
// Are you sure? Action
var sureAction;


function initPage() {
    doRequestAll("","");
}

function updatePage() {
}

function pageDisconnected() {
    // console.log("Status - Page Disconnected");
}

function SureNo() {
    document.getElementById('sure_dlg').style.display='none';
    if( sureAction == "log_save" ) disableElmt("log_save","log_cancel");
    doRequestAll();
 }



function SureYes() {

    clearSure();

    if( sureAction == "log_save" ) {

        x = document.getElementById('log_srl');
        doRequestWait(x.id, x.checked ? 't' : 'f');
        x = document.getElementById('log_baud');
        doRequestWait(x.id, x.value);
        x = document.getElementById('log_ser');
        if( x != null ) doRequestWait(x.id, x.checked ? 't' : 'f'); // TODO - javascript passer to avoid doing this
        x = document.getElementById('log_url');
        if( x != null ) doRequestWait(x.id, x.value);
        x = document.getElementById('log_key');
        if( x != null ) doRequestWait(x.id, x.value);
        x = document.getElementById('log_tick');
        if( x != null ) doRequestWait(x.id, x.checked ? 't' : 'f');
        x = document.getElementById('log_tick_int');
        if( x != null ) doRequestWait(x.id, x.value);
        x = document.getElementById('log_tags');
        doRequestWait(x.id, x.value);
        x = document.getElementById('log_level');
        doRequestWait(x.id, x.value);

        showLoader(document.getElementById('logger_box'));

        doRequest(sureAction,1,ClearLoader);

        disableElmt("log_save","log_cancel");

        return;
    }

    if( sureAction == "ota_save" ) {

        x = document.getElementById('ota_mode');
        doRequestWait(x.id, x.checked ? 't' : 'f');
        x = document.getElementById('ota_url');
        doRequestWait(x.id, x.value);
        x = document.getElementById('ota_user');
        doRequestWait(x.id, x.value);
        x = document.getElementById('ota_repo');
        doRequestWait(x.id, x.value);
        x = document.getElementById('ota_key');
        doRequestWait(x.id, x.value);
        x = document.getElementById('ota_skip');
        doRequestWait(x.id, x.checked ? 't' : 'f');
        x = document.getElementById('ota_ck_int');
        doRequestWait(x.id, x.value);

        showLoader(document.getElementById('ota_box'));

        doRequest(sureAction,1,ClearLoader);

        disableElmt("ota_save","ota_cancel");

        return;
    }

    if( sureAction == "btn_restart" ) {
        showLoader(document.getElementById('device_box'));
        doRequest(sureAction,1,reloadPage);

        return;
    }
    else {
        // All the settings resets
        showLoader(document.getElementById('reset_box'));
        doRequest(sureAction,1,reloadPage);

        return;
    }
}


