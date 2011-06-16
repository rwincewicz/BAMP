<?php

include 'charts/php-ofc-library/open-flash-chart.php';
include 'bamp.helper.inc';

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

  $date_array = db_list('bamp_field_results', 'date');

  $date_array[0] = 'all';

  $form['form']['date'] = array(
    '#type' => 'select',
    '#title' => t('Date'),
    '#options' => $date_array,
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

  $date_form = $form_state['values']['date'];

  $date = db_select('bamp_field_results', 'r')
      ->fields('r', array('date'))
      ->condition('id', $date_form, '=')
      ->execute()
      ->fetchField();

  $all_results = $form_state['values']['all'];

  if ($all_results == TRUE) {
    $no_results = 'all';
  }
  else {
    $no_results = $form_state['values']['list_no'];
  }

  drupal_goto('bamp/Charts/Chart2/' . $species . '/' . $trip . '/' . $site . '/' . $date);
}

function bamp_charts1_menu_callback() {
  return drupal_get_form('bamp_charts1_form');
}

function bamp_charts2_form($form, &$form_state, $species, $trip, $site, $date) {
  drupal_set_title(t('Chart 2'));

$select = db_select('bamp_field_results', 'f');
//$select->range(0,10);

if ($species == "") {
    $species = NULL;
  }

  if ($trip == "") {
    $trip = NULL;
  }

  if ($site == "") {
    $site = NULL;
  }
  
  if ($date == "") {
    $date = NULL;
  }

  if ($species !== NULL) {
    $select->condition('f.species', $species, '=');
  }

  if ($trip !== NULL) {
    $select->condition('f.trip_id', $trip, '=');
  }

  if ($site !== NULL) {
    $select->condition('f.site_name', $site, '=');
  }

  if ($date !== NULL) {
    $select->condition('f.date', $date, '=');
  }
  
  $select->addField('f', 'total_lice');
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  $entriesi = r_implode(',', ',', $entries);
  $entriese = explode(',', $entriesi);
  
//  Code to set up the variables for the chart
  $title = new title(t('BAMP chart'));
  
  foreach($entriese as &$value) {
    $value = (int)$value;
  }
  
  var_dump($entriese);
  
  $bar = new bar();
  $bar->set_values($entriese);
  $bar->colour('#1C9E05');

  $chart = new open_flash_chart();
  $chart->set_title($title);
  $chart->add_element($bar);

  $data = $chart->toPrettyString();
  
//  JS to render the chart and display it in a div
  drupal_add_js('sites/default/modules/bamp/charts/js/json/json2.js');
  drupal_add_js('sites/default/modules/bamp/charts/js/swfobject.js');
  $chart_data = '<script type="text/javascript">
  swfobject.embedSWF("sites/default/modules/bamp/charts/open-flash-chart.swf", "div_chart", "600", "600", "9.0.0", "expressInstall.swf", {"get-data":"get_data_1"}, {"wmode" : "transparent"} );
 
  function ofc_ready() {}
 
  function get_data_1()
  {
	  return JSON.stringify(data);
  }
 
  var data = ' . $data . ';
  </script>';

  $form = array();

//  Create the div for the chart to be placed in
  $form['chart'] = array(
    '#markup' => $chart_data . '<div id="div_chart"></div>',
  );

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Details'),
  );
//  Display search parameters
  $species_message = '<p>Species: ' . $species . '</br>';
  $trip_message = 'Trip: ' . $trip . '</br>';
  $site_message = 'Site: ' . $site . '</br>';
  $date_message = 'Date: ' . $date . '</br></p>';

  $form['form']['details'] = array(
    '#markup' => $species_message . $trip_message . $site_message . $date_message,
  );

  return $form;
}

function bamp_charts2_menu_callback($species=NULL, $trip=NULL, $site=NULL, $date=NULL) {
  return drupal_get_form('bamp_charts2_form', $species, $trip, $site, $date);
}