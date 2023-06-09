var chain_up = 'f';
var chain_down = 'f';
var current_rode = 0;
var water_line = 0;
var rode_len = 0;
var warn_limit_1 = 0;
var warn_limit_2 = 0;
var warn_limit_3 = 0;
var connected = false;


// https://github.com/naikus/svg-gauge
var rode_gauge = Gauge(
    document.getElementById("rode_gauge"),
    {
      min: 0,
      max: 100,
      dialStartAngle:360,
      dialEndAngle:90,
      viewBox: "10 40 100 60",
      label: function(value) {
        return value.toFixed(2) + "m";
      },
      color: function(value) {

        var colour;

        if(value <= water_line/100 ) {
          colour = "Tomato";
        }else if(value <= warn_limit_1/100) {
          colour = "Orange";
        }else if(value < warn_limit_2/100) {
          colour = "MediumSeaGreen";
        }else if(value < warn_limit_3/100) {
          colour = "Orange";
        }else {
          colour = "Tomato";
        }

        var elements = document.querySelectorAll('.gauge-container>.gauge .value-text');
        for(var i=0; i<elements.length; i++){
            elements[i].style.fill = colour
        }

        return colour;
      }
    }
  );

function initPage() {
    // console.log("Status - Initialize Page");
    doRequestAll(updatePage);
}

function pageDisconnected() {
    // console.log("Status - Page Disconnected");

    // Stop chain animation
    chain_down = 'f';
    chain_up = 'f';
    updatePage();
}

function updatePage() {
    // console.log("Status - Update Page");

    // Set gauge value and max in m
    var x = current_rode / 100;
    rode_gauge.setValue( x.toFixed(2));
    rode_gauge.setMaxValue(rode_len/100);

    // Set chain direction
    if( chain_up == 't' ) document.getElementById("chain").style.flexDirection = 'column-reverse';
    if( chain_down == 't' ) document.getElementById("chain").style.flexDirection = 'column';

    // Animate chain
    const collection = document.getElementsByClassName("chain__element");
    for (let i = 0; i < collection.length; i++) {
        collection[i].style.animationPlayState = ( chain_up == 't' || chain_down == 't' ) ? 'running' : 'paused';
    }
    
    x = rode_len / 100;
    var y = water_line / 100;
    document.getElementById("chain_spec").innerHTML = "Chain Length: " + x.toFixed(2) + "m / Water Line: " + y.toFixed(2) + "m";
}
