<?php

// $Id: bamp.audit.inc $

/**
 * @file
 * Code to create bamp audit forms
 */

function bamp_audit_form($form_state) {
  drupal_set_title(t('Audit menu'));

  $form = array();

  $form['form'] = array(
    '#markup' => '<p> This is the BAMP audit menu</p>',
  );

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Audit ID':
        $order = 'id';
        break;
      case 'Time':
        $order = 'time';
        break;
      case 'Edited table':
        $order = 'edited_table';
        break;
      case 'User':
        $order = 'user';
        break;
      default:
        $order = 'id';
    }
  }
  else {
    // Default sort
    $sort = 'ASC';
    $order = 'id';
  }

  $page_limit = 20;

  $header = array(
    array('data' => t('Audit ID'), 'field' => 'id'),
    array('data' => t('Time'), 'field' => 'time'),
    array('data' => t('Edited table'), 'field' => 'edited_table'),
    array('data' => t('User'), 'field' => 'user'));

  $select = db_select('bamp_audit', 'a')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($page_limit);

  $select->addField('a', 'id');
  $select->addField('a', 'time');
  $select->addField('a', 'edited_table');
  $select->addField('a', 'user');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
  }

  $nr = count($rows);

//  Loop to replace the ID text with a link to the edit form for that entry
  for ($i = 0; $i < $nr; $i++) {
    $id = $rows[$i]['id'];
    $rows[$i]['id'] = l($id, 'bamp/Audit/Field results/' . $id);
  }

  if ($rows == NULL) {
    $form['form']['none'] = array(
      '#markup' => '<p><h1>No results found</h1></p>',
    );
  }
  else {
    $form['Form'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );

    $form['pager'] = array(
      '#theme' => 'pager',
      '#quantity' => 10,
    );
  }

  return $form;
}

function bamp_audit_menu_callback() {
  return drupal_get_form('bamp_audit_form');
}

function bamp_audit_field_form($form, $form_state, $id) {
  drupal_set_title(t('Audit menu - Field samples'));

  $form = array();

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Audit details'),
  );

  if ($id == NULL) {
    $form['form']['id'] = array(
      '#markup' => 'No ID entered!',
    );
  }
  else {

    $query = db_select('bamp_audit', 'a');
    $query->condition('id', $id, '=');
    $query->addField('a', 'id');
    $query->addField('a', 'time');
    $query->addField('a', 'edited_table');
    $query->addField('a', 'old_values');
    $query->addField('a', 'new_values');
    $query->addField('a', 'user');
    $audit = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

//  var_dump($audit);

    $form['form']['id'] = array(
      '#markup' => 'ID: ' . $audit[0]['id'] . '<br />',
    );

    $form['form']['time'] = array(
      '#markup' => 'Time: ' . $audit[0]['time'] . '<br />',
    );

    $form['form']['edited_table'] = array(
      '#markup' => 'Edited table: ' . $audit[0]['edited_table'] . '<br />',
    );

    $form['form']['user'] = array(
      '#markup' => 'User: ' . $audit[0]['user'] . '<br />',
    );

    $header = array(t(''), t('Old values'), t('New values'));

    $old_array = explode(',', $audit['0']['old_values']);
    $new_array = explode(',', $audit['0']['new_values']);

    $select = db_select('bamp_field_results', 'f');

    $select->condition('f.id', $id, '=');

    $select->addField('f', 'id');
    $select->addField('f', 'fish_id');
    $select->addField('f', 'trip_id');
    $select->addField('f', 'sample_id');
    $select->addField('f', 'site_name');
    $select->addField('f', 'site_no');
    $select->addField('f', 'set_no');
    $select->addField('f', 'blind_no');
    $select->addField('f', 'to_lab');
    $select->addField('f', 'date');
    $select->addField('f', 'pen');
    $select->addField('f', 'input_name');
    $select->addField('f', 'route');
    $select->addField('f', 'rep_no');
    $select->addField('f', 'no_sampled');
    $select->addField('f', 'crew');
    $select->addField('f', 'species');
    $select->addField('f', 'height');
    $select->addField('f', 'length');
    $select->addField('f', 'weight');
    $select->addField('f', 'cop_lep');
    $select->addField('f', 'cop_cal');
    $select->addField('f', 'total_cop');
    $select->addField('f', 'chal_a');
    $select->addField('f', 'chal_b');
    $select->addField('f', 'total_chal');
    $select->addField('f', 'lep_pre_male');
    $select->addField('f', 'lep_pre_female');
    $select->addField('f', 'lep_pre_unid');
    $select->addField('f', 'total_lep_pre');
    $select->addField('f', 'lep_adult_male');
    $select->addField('f', 'lep_adult_female');
    $select->addField('f', 'lep_gravid');
    $select->addField('f', 'lep_adult_unid');
    $select->addField('f', 'total_lep_adult');
    $select->addField('f', 'total_lep');
    $select->addField('f', 'cal_pre_male');
    $select->addField('f', 'cal_pre_female');
    $select->addField('f', 'cal_pre_unid');
    $select->addField('f', 'total_cal_pre');
    $select->addField('f', 'cal_adult_male');
    $select->addField('f', 'cal_adult_female');
    $select->addField('f', 'cal_gravid');
    $select->addField('f', 'cal_adult_unid');
    $select->addField('f', 'total_cal_adult');
    $select->addField('f', 'total_cal');
    $select->addField('f', 'total_motile');
    $select->addField('f', 'total_lice');
    $select->addField('f', 'scar_chal');
    $select->addField('f', 'scar_mob');
    $select->addField('f', 'condition_factor');
    $select->addField('f', 'pred_mark');
    $select->addField('f', 'hem');
    $select->addField('f', 'mate_guard');
    $select->addField('f', 'pin_belly');
    $select->addField('f', 'tissue_damage');
    $select->addField('f', 'comments');
    $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

    $keys = array_keys($entries[0]);

    $row = array();

    $nk = count($keys);

    for ($i = 0; $i < $nk; $i++) {
      if ($old_array[$i] == $new_array[$i]) {
        $rows[] = array('value' => $keys[$i], 'old_id' => $old_array[$i], 'new_id' => $new_array[$i]);
      }
      else {
        $rows[] = array('value' => $keys[$i], 'old_id' => t('<b>' . $old_array[$i] . '</b>'), 'new_id' => t('<b>' . $new_array[$i] . '</b>'));
      }
    }

    $form['form']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  return $form;
}

function bamp_audit_field_menu_callback($id = NULL) {
  return drupal_get_form('bamp_audit_field_form', $id);
}