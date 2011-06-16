<?php

include 'charts/php-ofc-library/open-flash-chart.php';

function bamp_chart_form($form_state) {
  drupal_set_title(t('Chart menu'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p>This is the BAMP chart menu</p>',
  );

  return $form;
}

function bamp_charts_menu_callback() {
  return drupal_get_form('bamp_chart_form');
}

/**
 * Create search/browse page for the results
 * @param type $form
 * @param type $form_state
 * @return type 
 */

function bamp_charts1_form($form, &$form_state) {
  drupal_set_title(t('Chart 1'));
  
  $form = array();
  
  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search details'),
  );
  
  $species_array = db_list('bamp_field_results', 'species'); 
  
  $species_array[0] = 'all';
  
  $form['form']['species'] = array(
    '#type' => 'select',
    '#title' => t('Species'),
    '#options' => $species_array,
    '#default_value' => 0,
  );

  $trip_array = db_list('bamp_field_results', 'trip_id');
  
  $trip_array[0] = 'all';
  
  $form['form']['trip'] = array(
    '#type' => 'select',
    '#title' => t('Trip ID'),
    '#options' => $trip_array,
    '#default_value' => 0,
  );

  $site_array = db_list('bamp_field_results', 'site_name');
  
  $site_array[0] = 'all';
  
  $form['form']['site'] = array(
    '#type' => 'select',
    '#title' => t('Site ID'),
    '#options' => $site_array,
    '#default_value' => 0,
  );
  
  $form['form']['list_no'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of results per page'),
    '#size' => 5,
    '#default_value' => 20,
    '#required' => TRUE,
  );
  
  $form['form']['all'] = array(
    '#type' => 'checkbox',
    '#title' => t('All results'),
    '#default_value' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Chart'),
  );
  
  return $form;
}

function bamp_charts1_form_submit($form, &$form_state) {
 
  $file = 'data.json';
  
  $species_id = $form_state['values']['species'];
 
  $species = db_select('bamp_field_results', 'r')
      ->fields('r', array('species'))
      ->condition('id', $species_id, '=')
      ->execute()
      ->fetchField();
  
  $trip_id = $form_state['values']['trip'];
  
  $trip = db_select('bamp_field_results', 'r')
      ->fields('r', array('trip_id'))
      ->condition('id', $trip_id, '=')
      ->execute()
      ->fetchField();
  
  $site_name = $form_state['values']['site'];
  
  $site = db_select('bamp_field_results', 'r')
      ->fields('r', array('site_name'))
      ->condition('id', $site_name, '=')
      ->execute()
      ->fetchField();
  
  $all_results = $form_state['values']['all'];
  
  if ($all_results == TRUE) {
    $no_results = 'all';
  } else {
    $no_results = $form_state['values']['list_no']; 
  }
  
  drupal_goto('bamp/Charts/Chart2/' . $file);
}

function bamp_charts1_menu_callback() {
  return drupal_get_form('bamp_charts1_form');
}

function bamp_charts2_form($form, &$form_state) {
  drupal_set_title(t('Chart 2'));
  
  $title = new title(t('BAMP chart'));

$bar = new pie();
$bar->set_values( array(9,8,7,6,10,4,3,2,1) );
//$bar->set_colour( '#94D700' );

$chart = new open_flash_chart();
$chart->set_title( $title );
$chart->add_element( $bar );

$data = $chart->toPrettyString();
  
  drupal_add_js('sites/default/modules/bamp/charts/js/json/json2.js');
  drupal_add_js('sites/default/modules/bamp/charts/js/swfobject.js');
  $chart_data = '<script type="text/javascript">
  swfobject.embedSWF("sites/default/modules/bamp/charts/open-flash-chart.swf", "div_chart", "300", "300", "9.0.0", "expressInstall.swf", {"get-data":"get_data_1"} );
 
function ofc_ready() {}
 
function get_data_1()
{
	return JSON.stringify(data);
}
 
var data = ' . $data . ';
</script>';
  
  $form = array();

$form['form'] = array(
    '#markup' => $chart_data . '<div id="div_chart"></div>',
  );

  return $form;
}

function bamp_charts2_menu_callback() {
  return drupal_get_form('bamp_charts2_form');
}