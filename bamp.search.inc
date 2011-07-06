<?php

// $Id: bamp.search.inc $

/**
 * @file
 * Code to create bamp search forms
 */


/**
 *  Create top-level search menu page
 */

function bamp_search_form($form_state) {
  drupal_set_title(t('Search menu'));

  $form = array();

  $form['form'] = array(
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
  drupal_set_title(t('Field results search form'));

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
  
  $form['form']['lice_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show lice data'),
    '#default_value' => TRUE,
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
  
  $all_results = $form_state['values']['all'];
  
  if ($all_results == TRUE) {
    $no_results = 'all';
  } 
  else {
    $no_results = $form_state['values']['list_no']; 
  }
  
  if ($form_state['values']['lice_data'] == TRUE) {
    $data = 'y';
  }
  else {
    $data = 'n';
  }
  
  drupal_goto('bamp/Report forms/Field results/' . $species . '/' . $trip . '/' . $site . '/' . $no_results . '/' . $data);
}