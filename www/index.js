var chain_up = 'f';
var chain_down = 'f';
var current_rode = 0;
var water_line = 0;
var rode_len = 0;
var warn_limit_1 = 0;
var warn_limit_2 = 0;
var warn_limit_3 = 0;


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
        if(value <= water_line/100 ) {
          return "Tomato";
        }else if(value <= warn_limit_1/100) {
          return "Orange";
        }else if(value < warn_limit_2/100) {
          return "MediumSeaGreen";
        }else if(value < warn_limit_3/100) {
            return "Orange";
        }else {
          return "Tomato";
        }
      }
    }
  );

function initPage() {
    // console.log("Status - Initialize Page");

    updatePage();
}

function updatePage() {
    //console.log("Status - Update Page");

    // Set gauge value and max in m
    var x = current_rode / 100;
    if( current_rode != 0 ) rode_gauge.setValue( x.toFixed(2));
    if( rode_len != 0 ) rode_gauge.setMaxValue(rode_len/100);

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
