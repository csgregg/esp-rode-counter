// https://github.com/naikus/svg-gauge
var gauge2 = Gauge(
    document.getElementById("gauge2"),
    {
      min: 0,
      max: 50.00,
      dialStartAngle:360,
      dialEndAngle:90,
      viewBox: "10 40 100 60",
      label: function(value) {
        return value.toFixed(2) + "m";
      },
      color: function(value) {
        if(value < 2.00) {
          return "Tomato";
        }else if(value < 10.00) {
          return "Orange";
        }else if(value < 45.00) {
          return "MediumSeaGreen";
        }else if(value < 48.00) {
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
     console.log("Status - Update Page");

          // Set gauge value
          gauge2.setValue(20.1);

}



