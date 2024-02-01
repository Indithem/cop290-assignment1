
  document.addEventListener('DOMContentLoaded', function () {
    var sortCheckbox = document.getElementById("sortCheckbox");
    var sortOptions = document.getElementById("sortOptions");
    var sortButton = document.getElementById("highToLow");
    var resetButton = document.getElementById("reset");
    
    sortCheckbox.checked = false;
    sortCheckbox.addEventListener('change', function() {
      if (this.checked) {
        sortOptions.style.display = "block";
      } else {
        sortOptions.style.display = "none";
        sortButton.checked = false;
      }
    });

    resetButton.addEventListener('click',function(event) {
        sortButton.checked = false;
        sortOptions.style.display = "none";
        sortCheckbox.checked = false;
        });
    });
    
  
function make_slider(name,min_val,max_val, min_name, max_name){
  var slider = document.getElementById(name);
  var minInput = document.getElementById(min_name);
  var maxInput = document.getElementById(max_name);
  var resetButton = document.getElementById("reset");

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

  minInput.addEventListener('change', function () {
    slider.noUiSlider.set([this.value, null]);
  });

  maxInput.addEventListener('change', function () {
      slider.noUiSlider.set([null, this.value]);
  });

  resetButton.addEventListener('click',()=>{
    slider.noUiSlider.reset();
  })

  }

$(document).ready(function() {
  $(".loader").hide();
  $('#filters').submit(function(event) {
      event.preventDefault(); 
      $(".loader").show();
      var formData = $(this).serialize(); 
      $.ajax({
          type: 'POST',
          url: '/stocks_get_list',
          data: formData,
          success: function(response) {
              $('#stocks').html(response);
              $(".loader").hide();
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
      $(".loader").show();
      var formData = $(this).serialize(); 
      $.ajax({
          type: 'POST',
          url: '/stocks_get_graph',
          data: formData,
          success: function(response) {
            $(".loader").hide();   
            let plotly_data = JSON.parse(response);
            Plotly.react('plotlyDiv', plotly_data.data, plotly_data.layout);
          }
      })
    });
    var currentState = 'none';
    $('#toggle-btn').click(function() {
      if (currentState === 'all') {
        $('.custom-checkbox input[type="checkbox"]').prop('checked', false);
        $(this).text('Selected None');
        currentState = 'none';
      } else if (currentState === 'none') {
        $('.custom-checkbox input[type="checkbox"]').prop('checked', true);
        $(this).text('Selected All');
        currentState = 'all';
      } else {
        $('.custom-checkbox input[type="checkbox"]').prop('checked', true);
        $(this).text('Selected All');
        currentState = 'all';
      }
    });
    
    $('.custom-checkbox input[type="checkbox"]').change(function() {
      currentState = 'custom';
      $('#toggle-btn').text('Custom');
    });

  }});
  });
});