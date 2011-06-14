<?php

/**
 *  Create top-level search menu page
 */

function bamp_search_form($form_state) {
  drupal_set_title(t('Search menu'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p>This is the BAMP search menu</p>',
  );

  return $form;
}

function bamp_search_menu_callback() {
  return drupal_get_form('bamp_search_form');
}

/**
 * Create search/browse page for the results
 * @param type $form
 * @param type $form_state
 * @return type 
 */

function bamp_search_results_form($form, &$form_state) {
  drupal_set_title(t('Results search form'));

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
  
  $form['form']['list'] = array(
    '#markup' => '<p>Test: ' . implode(',', db_list('bamp_field_results', 'species')) . '</p>',
  );

  $site_array = db_list('bamp_field_results', 'site_name');
  
  $site_array[0] = 'all';
  
  $form['form']['site'] = array(
    '#type' => 'select',
    '#title' => t('Site ID'),
    '#options' => $site_array,
    '#default_value' => 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

function bamp_search_results_menu_callback() {
  return drupal_get_form('bamp_search_results_form');
}

function bamp_search_results_form_submit($form, &$form_state) {
 
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
  
  drupal_goto('bamp/Report forms/Results/' . $species . '/' . $trip . '/' . $site);
}