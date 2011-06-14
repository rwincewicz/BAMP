<?php

/**
 * Create BAMP report menu
 */
function bampreport_form($form_state) {
  drupal_set_title(t('Report menu'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p> This is the BAMP report menu</p>',
  );

  return $form;
}

function bampreport_menu_callback() {
  return drupal_get_form('bampreport_form');
}

function bamprepfarm_form($form, &$form_state) {
  drupal_set_title(t('Farm report form'));

  $form = array();

  $form['changethis'] = array(
    '#type' => 'select',
    '#options' => array(
      'one' => 'one',
      'two' => 'two',
      'three' => 'three',
    ),
    '#ajax' => array(
      'callback' => 'ajax_example_simplest_callback',
      'wrapper' => 'replace_textfield_div',
    ),
  );

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Details'),
    '#tree' => TRUE,
  );

  $form['form']['sid'] = array(
    '#type' => 'select',
    '#title' => 'Site ID',
    '#options' => db_list('bamp_sites', 'name'),
  );

  $form['form']['date'] = array(
    '#type' => 'select',
    '#title' => 'Date',
    '#options' => db_list('bamp_results', 'date'),
  );

  $form['form']['pen'] = array(
    '#type' => 'select',
    '#title' => 'Pen',
    '#options' => db_list('bamp_results', 'pen'),
    '#ajax' => array(
      'callback' => 'ajax_example_simplest_callback',
      'wrapper' => 'replace_textfield_div',
    )
  );


  // This entire form element will be replaced with an updated value.
  $form['replace_textfield'] = array(
    '#type' => 'textfield',
    '#title' => t("The default value will be changed"),
    '#description' => t("Say something about why you chose") . "'" .
    (!empty($form_state['values']['form']['pen']) ? $form_state['values']['form']['pen'] : t("Not changed yet")) . "'",
    '#prefix' => '<div id="replace_textfield_div">',
    '#suffix' => '</div>',
  );

  return $form;
}

function ajax_example_simplest_callback($form, $form_state) {
  // The form has already been submitted and updated. We can return the replaced
  // item as it is.
  return $form['replace_textfield'];
}

//  $form = array();
//
//  $form['form'] = array(
//    '#type' => 'fieldset',
//    '#title' => t('Details'),
//    '#tree' => TRUE,
//  );
//
//  $form['form']['sid'] = array(
//    '#type' => 'select',
//    '#title' => 'Site ID',
//    '#options' => db_list('bamp_sites', 'name'),
//  );
//
//  $form['form']['date'] = array(
//    '#type' => 'select',
//    '#title' => 'Date',
//    '#options' => db_list('bamp_results', 'date'),
//  );
//  
//  $form['form']['pen'] = array(
//    '#type' => 'select',
//    '#title' => 'Pen',
//    '#options' => db_list('bamp_results', 'pen'),
//  );
//
//  $form['submit'] = array(
//    '#type' => 'button',
//    '#value' => t('Submit'),
//    '#submit' => array('table_show'),
//    '#ajax' => array(
//      'callback' => 'build_table',
//      'wrapper' => 'table-div',
//      'method' => 'append',
//    ),
//  );
//  
//  $form['table'] = array(
//    '#title' => t('New table'),
//    '#prefix' => '<div id="table-div">',
//    '#suffix' => '</div>',
//    '#type' => 'fieldset',
//    '#description' => t('This is the new table'),
//  );
// 
//  return $form;
//}

function table_show($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function build_table($form, $form_state) {
  return $form['table'];
}

function bamprepfarm_menu_callback() {
  return drupal_get_form('bamprepfarm_form');
}

//function bamprepfarm_form_submit($form, &$form_state) {
//  drupal_set_message(t('Form submitted successfully'));
//
//  $form_state['rebuild'] = TRUE;
//  
//  $form['form']['table'] = array(
//    '#markup' => '<p> This is the BAMP report menu</p>',
//  );
//  
//  var_dump($form);
//  
//  return $form;
//  
//}

function bamprepres_form($form, &$form_state, $species_id, $trip_id, $site_id) {
  drupal_set_title(t('Results report form'));

  if ($species_id == NULL) {
    $species_message = 'Species: All<br />';
  } else {
    $species_message = 'Species: ' . $species_id . '<br />';
  }
  
  if ($trip_id == NULL) {
    $trip_message = 'Trip: All<br />';
  } else {
    $trip_message = 'Trip: ' . $trip_id . '<br />';
  }
  
  if ($site_id == NULL) {
    $site_message = 'Site: All<br />';
  } else {
    $site_message = 'Site: ' . $site_id . '<br />';
  }    

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Result ID':
        $order = 'id';
        break;
      case 'Fish ID':
        $order = 'fish_id';
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

  $form = array();

  $page_limit = 20;

  $header = array(
    array('data' => t('Result ID'), 'field' => 'id'),
    array('data' => t('Fish ID'), 'field' => 'fish_id'),
    t('Trip ID'),
    t('Site ID'),
    t('Date'),
    t('Species'),
    t('Caligus - Lab'),
    t('Chalimus - Field'),
    t('Species - Lab'));

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search details'),
  ); 

  $select = db_select('bamp_field_results', 'f')
          ->extend('TableSort')
          ->extend("PagerDefault")
          ->limit($page_limit);
  
  if ($species_id !== NULL) {
         $select->condition('f.species', $species_id, '=');
  }
  if ($site_id !== NULL) {
         $select->condition('f.site_name', $site_id, '=');
  }
  if ($trip_id !== NULL) {
         $select->condition('f.trip_id', $trip_id, '=');
  }   
  
  $form['form']['details'] = array(
        '#markup' => $species_message . $trip_message . $site_message,
      );
  
  $select->leftJoin('bamp_lab_results', 'l', 'f.fish_id = l.fish_id');
  $select->addField('f', 'id');
  $select->addField('f', 'fish_id');
  $select->addField('f', 'trip_id');
  $select->addField('f', 'site_name');
  $select->addField('f', 'date');
  $select->addField('f', 'species');
  $select->addField('l', 'caligus');
  $select->addField('f', 'chalimus');
  $select->addField('l', 'species');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();
  
  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
  }

  var_dump($rows);
  
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

  $sum_chalimus = 0;

  $list_no = count($rows);

  for ($i = 0; $i < $list_no; $i++) {

    $sum_chalimus += $rows[$i]['chalimus'];
  }

  $ave_chalimus = $sum_chalimus / $page_limit;

  $sum_caligus = 0;

  for ($i = 0; $i < $list_no; $i++) {

    $sum_caligus += $rows[$i]['caligus'];
  }

  $ave_caligus = $sum_caligus / $page_limit;

  $sum = array(
    array('sum' => 'Total', 'two' => 'N/A', 'three' => 'N/A', 'four' => 'N/A', 'five' => 'N/A', 'six' => 'N/A', 'caligus' => $sum_caligus, 'chalimus' => $sum_chalimus, 'nine' => 'N/A'),
    array('sum' => 'Average', 'two' => 'N/A', 'three' => 'N/A', 'four' => 'N/A', 'five' => 'N/A', 'six' => 'N/A', 'caligus' => $ave_caligus, 'chalimus' => $ave_chalimus, 'nine' => 'N/A'));

//  var_dump($sum);

  $header_calc = array(
    t(''),
    t('Fish ID'),
    t('Trip ID'),
    t('Site ID'),
    t('Date'),
    t('Species'),
    t('Caligus - Lab'),
    t('Chalimus - Field'),
    t('Species - Lab'));

  $form['sum'] = array(
    '#theme' => 'table',
    '#header' => $header_calc,
    '#rows' => $sum,
  );

  $form['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => 10,
  );
  }
  
  return $form;
}

function bamprepres_menu_callback($species_id=NULL, $trip_id=NULL, $site_id=NULL) {
  return drupal_get_form('bamprepres_form', $species_id, $trip_id, $site_id);
}

/**
 * Report for sites
 */
function bamprepsite_form($form, &$form_state) {
  drupal_set_title(t('Site report form'));

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Result ID':
        $order = 'id';
        break;
      case 'Fish ID':
        $order = 'fish_id';
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

  $form = array();

  $page_limit = 20;

  $header = array(
    array('data' => t('Site ID'), 'field' => 'id'),
    array('data' => t('Crawford Site ID'), 'field' => 'craw_site_id'),
    t('Name'),
    t('Min lat'),
    t('Min lon'),
    t('Max lat'),
    t('Max lon'),
    t('Farm ID'),
    t('Zone'),
    t('Area'));

  $select = db_select('bamp_sites', 's')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($page_limit);
//  $select->leftJoin('bamp_field_results', 'f', 'r.fish_id = f.fish_id');
//  $select->leftJoin('bamp_lab_results', 'l', 'r.fish_id = l.fish_id');
  $select->addField('s', 'id');
  $select->addField('s', 'craw_site_id');
  $select->addField('s', 'name');
  $select->addField('s', 'min_latitude');
  $select->addField('s', 'min_longitude');
  $select->addField('s', 'max_latitude');
  $select->addField('s', 'max_longitude');
  $select->addField('s', 'farm_id');
  $select->addField('s', 'zone');
  $select->addField('s', 'area');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
  }

  $form['Form'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

//  var_dump($rows);

  $form['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => 10,
  );

  return $form;
}

function bamprepsite_menu_callback() {
  return drupal_get_form('bamprepsite_form');
}