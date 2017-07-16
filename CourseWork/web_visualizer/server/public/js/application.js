(function() {
    'use strict';
    var dialogButton = document.querySelector('.dialog-button');
    var dialog = document.querySelector('#dialog');
    if (! dialog.showModal) {
        dialogPolyfill.registerDialog(dialog);
    }
    dialogButton.addEventListener('click', function() {
        dialog.showModal();
    });
    dialog.querySelector('button:not([disabled])')
        .addEventListener('click', function() {
            dialog.close();
        });
}());

$(".mdl-textfield").on("keyup", function (ev) {
    console.log("text field changed");
    isValid = true;

    $(".mdl-textfield").each(function(i) {
        console.log("i=", i);
        if($(this).hasClass('is-invalid') || !$(this).hasClass('is-dirty')) {
            isValid = false
        }
    });

    if(isValid)
        $("#btn_pack").prop('disabled', false);
    else
        $("#btn_pack").prop('disabled', true);

});


$('#btn_pack').click(function(evt){
    evt.preventDefault();
    var l = $('#length_input').val();
    var w = $('#width_input').val();
    var h = $('#height_input').val();
    var mol_id = $('#molecules').prop("selectedIndex") + 1;


    $('#p2').show();
    $("#btn_pack").prop('disabled', true);


    $.ajax({
        url: `/get/lattice/${mol_id}/?length=${l}&height=${h}&width=${w}`,
        type: 'GET',
        dataType: 'json',
        success: function (data, textStatus) {
            $("#btn_pack").prop('disabled', false);
            $('#p2').hide();

            // main logic

            // ...

            console.log('data: ', data);
            console.log('text status: ', textStatus);

            // logic end


            // if(data['result'] == 'rntime error') {
            //     console.log(`debug_info:\n${data['debug']}`);
            //     console.error(`runtime error: ${data['errors']}`);
            //     return;
            // }
            // else if(data['result'] == 'execution error') {
            //     console.error(`execution error: ${data['error']}`);
            //     return;
            // }
            //
            // if(typeof(Storage) !== 'undefined') {
            //     var storage;
            //
            //     if(typeof(sessionStorage) !== 'undefined')
            //         storage = sessionStorage;
            //     else if(typeof(localStorage) !== 'undefined')
            //         storage = localStorage;
            //     else
            //         alert('unsupported type of browser storage.\nTry to use another browser (like Google Chrome, Firefox, Opera and etc.)');
            //
            //     if(typeof(storage.getItem('lattice_data')) !== 'undefined') {
            //         storage.removeItem('lattice_data');
            //     }
            //
            //     storage.setItem('lattice_data', data['data']);
            //
            //     $("#btn_pack").prop('disabled', false);
            //
            //     var download_href = `/download/lattice/${mol_id}/?length=${l}&width=${w}&height=${h}`;
            //     var dropdownItems = [
            //         {'href': `${download_href}&format=json`, 'text': 'JSON'},
            //         {'href': `${download_href}&format=mol2`, 'text': 'MOL2'},
            //         {'href': `${download_href}&format=pdb`, 'text': 'PDB'},
            //         {'href': `${download_href}&format=sdf`, 'text': 'SDF'}
            //     ];
            //     var ddHtml = createDropdown('Download', dropdownItems);
            //
            //     $('#controls').append(ddHtml);
            //     $('#controls').append(`<a href='visualizer.html' target='_blank'>Preview</a><br/>`);
            //
            //     $('.dropdown-toggle').dropdown();
            // }
            //
            // else {
            //     alert('This browser don\'t support local storage.\nPlease try another one (like Google Chrome, Firefox, Opera and etc.)');
            // }

        }
    });

});

