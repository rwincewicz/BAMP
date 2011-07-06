<?php

// $Id: bamp.input.inc $

/**
 * @file
 * Code to create bamp input forms
 */

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Create BAMP input menu
 */
function bamp_input_menu_callback() {
  drupal_set_title(t('BAMP Input Menu'));

  $form['form'] = array(
    '#markup' => '<p> This is the BAMP input menu</p>',
  );

  return $form;
}

/**
 * Create BAMP farm input form
 */
function bamp_farm_input_form($form_state) {
  drupal_set_title(t('Farm input form'));

  $form = array();

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Farm details'),
  );

  $form['form']['fid'] = array(
    '#markup' => '<p><h4><b> Farm id: ' . next_id_bamp('bamp_farm') . '</b></h4></p>',
  );

  $form['form']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Farm name'),
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['form']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Farm description'),
    '#required' => FALSE,
  );

  $form['form']['treatments'] = array(
    '#type' => 'textfield',
    '#title' => t('Treatments'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#executes_submit_callback' => TRUE,
  );

  return $form;
}

function bamp_farm_input_menu_callback() {
  return drupal_get_form('bamp_farm_input_form');
}

function bamp_farm_input_form_submit($form, &$form_state) {
  drupal_set_message(t('Form submitted successfully'));

  $farm = db_insert('bamp_farm')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'treatments' => $form_state['values']['treatments'],
      ))
      ->execute();
}

/**
 * Create BAMP site input form
 */
function bamp_site_input_form($form_state) {
  drupal_set_title(t('Site input form'));

  $form = array();

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site details'),
  );

  $form['form']['siid'] = array(
    '#markup' => '<p><h4><b> Site id: ' . next_id_bamp('bamp_sites') . '</b></h4></p>',
  );

  $form['form']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#required' => TRUE,
  );

  $form['form']['type'] = array(
    '#type' => 'textfield',
    '#title' => 'Type',
    '#required' => TRUE,
  );

  $form['form']['fid'] = array(
    '#type' => 'textfield',
    '#title' => 'Farm ID',
    '#required' => TRUE,
  );

  $form['form']['min_lon'] = array(
    '#type' => 'textfield',
    '#title' => 'Min longitude',
    '#required' => TRUE,
  );

  $form['form']['min_lat'] = array(
    '#type' => 'textfield',
    '#title' => 'Min latitude',
    '#required' => TRUE,
  );

  $form['form']['max_lon'] = array(
    '#type' => 'textfield',
    '#title' => 'Max longitude',
    '#required' => TRUE,
  );

  $form['form']['max_lat'] = array(
    '#type' => 'textfield',
    '#title' => 'Max latitude',
    '#required' => TRUE,
  );

  $form['form']['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Description',
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#executes_submit_callback' => TRUE,
  );

  return $form;
}

function bamp_site_input_menu_callback() {
  return drupal_get_form('bamp_site_input_form');
}

function bamp_site_input_form_submit($form, &$form_state) {
  drupal_set_message(t('Form submitted successfully'));

  $farm = db_insert('bamp_sites')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'type' => $form_state['values']['type'],
        'min_longitude' => $form_state['values']['min_lon'],
        'min_latitude' => $form_state['values']['min_lat'],
        'max_longitude' => $form_state['values']['max_lon'],
        'max_latitude' => $form_state['values']['max_lat'],
        'description' => $form_state['values']['description'],
        'farm_id' => $form_state['values']['fid'],
      ))
      ->execute();
}

/**
 * Create BAMP trip input form
 */
function bamp_trip_input_form($form_state) {
  drupal_set_title(t('Trip input form'));

  $form = array();

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trip details'),
  );

  $form['form']['tid'] = array(
    '#markup' => '<p><h4><b> Trip id: ' . next_id_bamp('bamp_trip') . '</b></h4></p>',
  );

  $form['form']['sid'] = array(
    '#type' => 'textfield',
    '#title' => 'Site ID',
    '#required' => TRUE,
  );

  $form['form']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#required' => TRUE,
  );

  $form['form']['sdate'] = array(
    '#type' => 'date',
    '#title' => 'Start date',
  );

  $form['form']['edate'] = array(
    '#type' => 'date',
    '#title' => 'End date',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#executes_submit_callback' => TRUE,
  );

  return $form;
}

function bamp_trip_input_menu_callback() {
  return drupal_get_form('bamp_trip_input_form');
}

/**
 * Create BAMP lab input form
 */
function bamp_lab_input_form($form_state) {
  drupal_set_title(t('Lab input form'));

  $form = array();

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lab details'),
  );

  $form['form']['lid'] = array(
    '#markup' => '<p><h4><b> Lab id: ' . next_id_bamp('bamp_lab') . '</b></h4></p>',
  );

  $form['form']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#required' => TRUE,
  );

  $form['form']['notes'] = array(
    '#type' => 'textarea',
    '#title' => 'Notes',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#executes_submit_callback' => TRUE,
  );

  return $form;
}

function bamp_lab_input_menu_callback() {
  return drupal_get_form('bamp_lab_input_form');
}

/**
 * Create BAMP result input form
 */
function bamp_field_input_form($form_state) {
  drupal_set_title(t('Field results input form'));

  $form = array();

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Result details'),
  );

  $form['form']['rid'] = array(
    '#markup' => '<p><h4><b> Result id: ' . next_id_bamp('bamp_field_results') . '</b></h4></p>',
  );

  $form['form']['sid'] = array(
    '#type' => 'textfield',
    '#title' => 'Site ID',
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['form']['fid'] = array(
    '#type' => 'textfield',
    '#title' => 'Fish ID',
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['form']['pen'] = array(
    '#type' => 'textfield',
    '#title' => 'Pen',
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['form']['tot_fish'] = array(
    '#type' => 'textfield',
    '#title' => 'Total number of fish',
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['form']['notes'] = array(
    '#type' => 'textarea',
    '#title' => 'Comments',
  );

  $form['form']['date'] = array(
    '#type' => 'date',
    '#title' => 'Date',
    '#required' => TRUE,
  );

  $form['form']['mid'] = array(
    '#type' => 'textfield',
    '#title' => 'Method ID',
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['form']['lid'] = array(
    '#type' => 'textfield',
    '#title' => 'Lab ID',
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['form']['user'] = array(
    '#type' => 'textfield',
    '#title' => 'User',
    '#required' => TRUE,
  );

  $form['form']['cal'] = array(
    '#type' => 'textfield',
    '#title' => 'Caligus',
    '#size' => 5,
  );

  $form['form']['chal'] = array(
    '#type' => 'textfield',
    '#title' => 'Chalimus',
    '#size' => 5,
  );

  $form['form']['pre'] = array(
    '#type' => 'textfield',
    '#title' => 'Pre-adult',
    '#size' => 5,
  );

  $form['form']['male'] = array(
    '#type' => 'textfield',
    '#title' => 'Male',
    '#size' => 5,
  );

  $form['form']['female'] = array(
    '#type' => 'textfield',
    '#title' => 'Female',
    '#size' => 5,
  );

  $form['form']['grav'] = array(
    '#type' => 'textfield',
    '#title' => 'Gravid',
    '#size' => 5,
  );

  $form['form']['data'] = array(
    '#type' => 'textfield',
    '#title' => 'Total lice',
    '#size' => 5,
  );

  $form['form']['scar'] = array(
    '#type' => 'textarea',
    '#title' => 'Scarring',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#executes_submit_callback' => TRUE,
  );

  return $form;
}

function bamp_field_input_menu_callback() {
  return drupal_get_form('bamp_field_input_form');
}