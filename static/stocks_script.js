
  document.addEventListener('DOMContentLoaded', function () {
    var avgPriceCheckbox = document.getElementById("avgPriceCheckbox");
    var avgPriceMinLabel = document.getElementById("averagePriceMinLabel");
    var avgPriceMaxLabel = document.getElementById("averagePriceMaxLabel");
    var Volumecheckbox = document.getElementById("Volumecheckbox");
    var avgPriceSlider = document.getElementById("avgPriceSlider");
    var Volumeslider = document.getElementById("Volumeslider");
    var noneOptionCheckbox = document.getElementById("noneOptionCheckbox");
    var sortCheckbox = document.getElementById("sortCheckbox");
    var sortOptions = document.getElementById("sortOptions");
    var sortButtons = document.getElementById("sortButtons");
    var Vol_minLabel = document.getElementById("Volumeminlabel"); // Corrected ID
    var Volumemaxlabel = document.getElementById("Volumemaxlabel"); // Corrected ID
 
    avgPriceCheckbox.addEventListener("change", function() {
      if (avgPriceCheckbox.checked) {
          avgPriceSlider.style.display = "block";
          avgPriceMinLabel.style.display = "inline-block";
          avgPriceMaxLabel.style.display = "inline-block";
      } else {
          avgPriceSlider.style.display = "none";
          avgPriceMinLabel.style.display = "none";
          avgPriceMaxLabel.style.display = "none";
      }
    });
  
    Volumecheckbox.addEventListener("change", function() {
      if (Volumecheckbox.checked) {
        Volumeslider.style.display = "block";
        Vol_minLabel.style.display = "inline-block";
        Volumemaxlabel.style.display = "inline-block";
      } else {
        Volumeslider.style.display = "none";
        Vol_minLabel.style.display = "none";
        Volumemaxlabel.style.display = "none";
      }
    });
  
    sortCheckbox.addEventListener("change", function() {
      if (sortCheckbox.checked) {
        sortOptions.style.display = "block";
        sortButtons.style.display = "block";
      } else {
        sortOptions.style.display = "none";
        sortButtons.style.display = "none";
      }
    });
  
    noneOptionCheckbox.addEventListener("change", function() {
      if (noneOptionCheckbox.checked) {
        sortCheckbox.checked = false;
        avgPriceCheckbox.checked = false;
        Volumecheckbox.checked = false;
        lowToHigh.checked = false;
        highToLow.checked = false;
        avgPriceSlider.style.display = "none";
        Volumeslider.style.display = "none";
        sortButtons.style.display = "none";
        Volumemaxlabel.style.display ="none";
        Vol_minLabel.style.display ="none";
        avgPriceMinLabel.style.display = "none";
        avgPriceMaxLabel.style.display = "none";
      }
    });
    
  });
  
function make_slider(name,min_val,max_val, min_name, max_name){
  var slider = document.getElementById(name);
  var minInput = document.getElementById(min_name);
  var maxInput = document.getElementById(max_name);

  noUiSlider.create(slider, {
    start: [min_val, max_val],
    connect: true,
    range: {
      'min': min_val,
      'max': max_val
    }
  });

  slider.noUiSlider.on('update', function (values, handle) {
    var value = values.map(function (val) {
      return Number(val).toFixed(2);
    });

    minInput.value = value[0];
    maxInput.value = value[1];
  });
  }

$(document).ready(function() {
  $('#filters').submit(function(event) {
      event.preventDefault(); 
      var formData = $(this).serialize(); 
      $.ajax({
          type: 'POST',
          url: '/stocks_get_list',
          data: formData,
          success: function(response) {
              $('#stocks').html(response);
          
    $('#search').keyup(function() {
      var input = $(this).val();
      if (input.length > 0) {
        $('#stocks span').each(function() {
          if ($(this).text().toLowerCase().includes(input.toLowerCase())) {
            $(this).show();
          } else {
            $(this).hide();
          }
        });
      } else {
        $('#stocks span').show();
    }});
    
    $('#stocks-form').submit(function(event) {
      event.preventDefault(); 
      var formData = $(this).serialize(); 
      $.ajax({
          type: 'POST',
          url: '/stocks_get_graph',
          data: formData,
          success: function(response) {
            let plotly_data = JSON.parse(response);
            Plotly.react('plotlyDiv', plotly_data.data, plotly_data.layout);
          }
      })
    });
  }


      }
      );
  });
});