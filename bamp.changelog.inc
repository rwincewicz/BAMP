<?php

// $Id: bamp.changelog.inc $

/**
 * @file
 * Code to create bamp change log forms
 */
function bamp_changelog_form($form_state) {
  drupal_set_title(t('Change log'));

  $form = array();

  $form['form'] = array(
    '#markup' => '<p> This is the BAMP change log</p>',
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
      case 'Change log ID':
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
    array('data' => t('Change log ID'), 'field' => 'id'),
    array('data' => t('Time'), 'field' => 'time'),
    array('data' => t('Edited table'), 'field' => 'edited_table'),
    array('data' => t('User'), 'field' => 'user'));

  $select = db_select('bamp_changelog', 'a')
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
    $rows[$i]['id'] = l($id, 'bamp/Changelog/Results/' . $id);
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

function bamp_changelog_menu_callback() {
  return drupal_get_form('bamp_changelog_form');
}

function bamp_changelog_results_form($form, $form_state, $id) {
  drupal_set_title(t('Change log menu'));

  $form = array();

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Change log details'),
  );

  if ($id == NULL) {
    $form['form']['id'] = array(
      '#markup' => 'No ID entered!',
    );
  }
  else {

    $query = db_select('bamp_changelog', 'a');
    $query->condition('id', $id, '=');
    $query->addField('a', 'id');
    $query->addField('a', 'time');
    $query->addField('a', 'edited_table');
    $query->addField('a', 'old_values');
    $query->addField('a', 'new_values');
    $query->addField('a', 'user');
    $changelog = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $form['form']['id'] = array(
      '#markup' => 'ID: ' . $changelog[0]['id'] . '<br />',
    );

    $form['form']['time'] = array(
      '#markup' => 'Time: ' . $changelog[0]['time'] . '<br />',
    );

    $form['form']['edited_table'] = array(
      '#markup' => 'Edited table: ' . $changelog[0]['edited_table'] . '<br />',
    );

    $form['form']['user'] = array(
      '#markup' => 'User: ' . $changelog[0]['user'] . '<br />',
    );

    $header = array(t(''), t('Old values'), t('New values'));

    $old_array = explode(',', $changelog['0']['old_values']);
    $new_array = explode(',', $changelog['0']['new_values']);

    switch ($changelog[0]['edited_table']) {

      case 'bamp_field_results':

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
        break;

      case 'bamp_sites':

        $select = db_select('bamp_sites', 's');

        $select->condition('s.id', $id, '=');

        $select->addField('s', 'id');
        $select->addField('s', 'craw_site_id');
        $select->addField('s', 'name');
        $select->addField('s', 'min_latitude');
        $select->addField('s', 'min_longitude');
        $select->addField('s', 'type');
        $select->addField('s', 'description');
        $select->addField('s', 'farm_id');
        $select->addField('s', 'zone');
        $select->addField('s', 'area');
        $select->addField('s', 'rep');
        $select->addField('s', 'route');
        $select->addField('s', 'distance');
        $select->addField('s', 'date');
        $select->addField('s', 'year');
        $select->addField('s', 'group_name');
        $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
        break;

      case 'bamp_weather':

        $select = db_select('bamp_weather', 'w');

        $select->condition('w.id', $id, '=');

        $select->addField('w', 'id');
        $select->addField('w', 'date');
        $select->addField('w', 'time');
        $select->addField('w', 'site_name');
        $select->addField('w', 'temp');
        $select->addField('w', 'temp_hi');
        $select->addField('w', 'temp_low');
        $select->addField('w', 'hum');
        $select->addField('w', 'dew_pt');
        $select->addField('w', 'wind_speed');
        $select->addField('w', 'wind_hi');
        $select->addField('w', 'wind_dir');
        $select->addField('w', 'wind_dir_deg');
        $select->addField('w', 'rain');
        $select->addField('w', 'bar');
        $select->addField('w', 'arc_int');
        $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
        break;

      case 'bamp_environment':

        $select = db_select('bamp_environment', 'e');

        $select->condition('e.id', $id, '=');

        $select->addField('e', 'id');
        $select->addField('e', 'date');
        $select->addField('e', 'site_name');
        $select->addField('e', 'temp');
        $select->addField('e', 'oxygen');
        $select->addField('e', 'sal');
        $select->addField('e', 'depth');
        $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
        break;

      case 'bamp_trip':

        $select = db_select('bamp_trip', 't');

        $select->condition('t.id', $id, '=');

        $select->addField('t', 'id');
        $select->addField('t', 'start_date');
        $select->addField('t', 'start_time');
        $select->addField('t', 'site_id');
        $select->addField('t', 'name');
        $select->addField('t', 'vessel');
        $select->addField('t', 'method');
        $select->addField('t', 'comments');
        $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
        break;

      case 'bamp_lab_results':
        
        $table = 'bamp_lab_results';

        $result = db_query("SHOW COLUMNS FROM $table");
        $query = $result->fetchAll();

        $select = db_select($table, 'l');

        $select->condition('l.id', $id, '=');

        $nq = count($query);

        for ($i = 0; $i < $nq; $i++) {

          $field = $query[$i];

          $fields[] = $field->field;

          $fieldsi = $fields[$i];

          $select->addField('l', $fieldsi);
        }

        $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
        break;
    }

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

function bamp_changelog_results_menu_callback($id = NULL) {
  return drupal_get_form('bamp_changelog_results_form', $id);
}