/**
 * Created by alex on 16.07.17.
 */

(function() {
    sliderClicked = false;


    function updateSlider(ev) {
        $slider = $(".slider");

        sliderLeft = $slider.offset().left;
        sliderWidth = $slider.width();

        percent = (ev.pageX - sliderLeft) / sliderWidth * 100;

        $slider.children(".slider-fg").css({width: `${percent}%`});
        $slider.prev().val(percent.toFixed(2));
    }

    $(".controls__hide").on("click", function() {

        if($(this).prev().hasClass('closed'))
            $(this).text("Hide controls");
        else
            $(this).text("Show controls");

        $(this).prev().toggleClass("closed");
    });

    $(".slider").on("mousedown", function (ev) {
        sliderClicked = true;

        updateSlider(ev);
    });

    $(".slider").on("mouseup", function (ev) {
        sliderClicked = false;
    });

    $(".slider").on("mousemove", function (ev) {
        if(sliderClicked)
            updateSlider(ev);
    });


})();
