document.addEventListener("DOMContentLoaded", function() {
    var avgPriceCheckbox = document.getElementById("avgPriceCheckbox");
    var peRatioCheckbox = document.getElementById("peRatioCheckbox");
    var averagePriceMin = document.getElementById("averagePriceMin");
    var averagePriceMax = document.getElementById("averagePriceMax");
    var peRatioMin = document.getElementById("peRatioMin");
    var peRatioMax = document.getElementById("peRatioMax");
    var sortOptions = document.getElementsByName("sortOption");
    var noneOption = document.getElementsByName("noneOption")[0];
  
    // Toggle average price range inputs visibility
    avgPriceCheckbox.addEventListener("change", function() {
        if (avgPriceCheckbox.checked) {
            averagePriceMin.style.display = "inline-block";
            averagePriceMax.style.display = "inline-block";
        } else {
            averagePriceMin.style.display = "none";
            averagePriceMax.style.display = "none";
        }
    });
  
    // Toggle P/E ratio range inputs visibility
    peRatioCheckbox.addEventListener("change", function() {
        if (peRatioCheckbox.checked) {
            peRatioMin.style.display = "inline-block";
            peRatioMax.style.display = "inline-block";
        } else {
            peRatioMin.style.display = "none";
            peRatioMax.style.display = "none";
        }
    });
  
    // Handle "None" option clearing other selections
    noneOption.addEventListener("change", function() {
      if (noneOption.checked) {
        // Clear other checkboxes and radio buttons
        avgPriceCheckbox.checked = false;
        peRatioCheckbox.checked = false;
        sortOptions.forEach(function(option) {
          option.checked = false;
        });
  
        // Hide related input fields
        averagePriceMin.style.display = "none";
        averagePriceMax.style.display = "none";
        peRatioMin.style.display = "none";
        peRatioMax.style.display = "none";
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
            'min': 0,
            'max': 100
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