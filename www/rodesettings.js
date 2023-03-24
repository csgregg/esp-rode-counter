// Are you sure? Action
var sureAction;

function initPage() {
    // console.log("Status - Initialize Page");

    updatePage();
}

function updatePage() {
    // console.log("Status - Update Page");

}


function SureNo() {
    document.getElementById('sure_dlg').style.display='none';
    if( sureAction == "rode_save" ) disableElmt("rode_save","rode_cancel");
    doRequestAll();
}


function SureYes() {

    clearSure();

    if( sureAction == "rode_save" ) {

        x = document.getElementById('windlass_dia');
        doRequestWait(x.id, x.value);
        x = document.getElementById('windlass_rpm');
        doRequestWait(x.id, x.value);
        x = document.getElementById('windlass_rev');
        doRequestWait(x.id, x.checked ? 't' : 'f');
        x = document.getElementById('chain_len');
        doRequestWait(x.id, x.value);
        x = document.getElementById('water_line');
        doRequestWait(x.id, x.value);

        showLoader(document.getElementById('windchain_box'));

        doRequest(sureAction,1,ClearLoader);

        disableElmt("rode_save","rode_cancel");
    }

    if( sureAction == "btn_reset" ) {
        showLoader(document.getElementById('reset_box'));
        doRequest(sureAction,1,reloadPage);
    }

}

