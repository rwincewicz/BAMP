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

function table_show($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function build_table($form, $form_state) {
  return $form['table'];
}

function bamprepfarm_menu_callback() {
  return drupal_get_form('bamprepfarm_form');
}

function bamprepres_form($form, &$form_state, $species_id, $trip_id, $site_id, $no_results) {
  drupal_set_title(t('Combined results report form'));

  if ($species_id == NULL) {
    $species_message = 'Species: All<br />';
  }
  else {
    $species_message = 'Species: ' . $species_id . '<br />';
  }

  if ($trip_id == NULL) {
    $trip_message = 'Trip: All<br />';
  }
  else {
    $trip_message = 'Trip: ' . $trip_id . '<br />';
  }

  if ($site_id == NULL) {
    $site_message = 'Site: All<br />';
  }
  else {
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

  $header = array(
    array('data' => t('Result ID'), 'field' => 'id'),
    array('data' => t('Fish ID'), 'field' => 'fish_id'),
    t('Trip ID'),
    t('Site ID'),
    t('Lab'),
    t('Date'),
    t('Crew'),
    t('Species'),
    t('Weight/kg'),
    t('Length/mm'),
    t('Caligus - Lab'),
    t('Chalimus - Field'),
    t('Total lice'),
    t('Species - Lab'));

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search details'),
  );

  if ($no_results == 'all') {
    $list_no = next_id('bamp_field_results');
    $no_message = 'Showing all results</br>';
  } else {
    $list_no = $no_results;
    $no_message = 'Showing ' . $no_results . ' results per page </br>';
  }
  
  $select = db_select('bamp_field_results', 'f')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($list_no);

  if ($species_id == "") {
    $species_id = NULL;
  }

  if ($trip_id == "") {
    $trip_id = NULL;
  }

  if ($site_id == "") {
    $site_id = NULL;
  }

  if ($species_id !== NULL) {
    $select->condition('f.species', $species_id, '=');
  }

  if ($trip_id !== NULL) {
    $select->condition('f.trip_id', $trip_id, '=');
  }

  if ($site_id !== NULL) {
    $select->condition('f.site_name', $site_id, '=');
  }

  $form['form']['details'] = array(
    '#markup' => $species_message . $trip_message . $site_message . $no_message,
  );
  
  $form['form']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Download CSV')
    );
  
  $select->leftJoin('bamp_lab_results', 'l', 'f.fish_id = l.fish_id');
  $select->addField('f', 'id');
  $select->addField('f', 'fish_id');
  $select->addField('f', 'trip_id');
  $select->addField('f', 'site_name');
  $select->addField('f', 'to_lab');
  $select->addField('f', 'date');
  $select->addField('f', 'crew');
  $select->addField('f', 'species');
  $select->addField('l', 'weight');
  $select->addField('l', 'length');
  $select->addField('l', 'caligus');
  $select->addField('f', 'chalimus');
  $select->addField('f', 'total_lice');
  $select->addField('l', 'species');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
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

    if ($no_results == 'all') {
      $no_results = count($rows);
    }
    
    $sum_weight = bamp_sum('weight', $rows);

    $ave_weight = round($sum_weight / $no_results, 2);

    $sum_length = bamp_sum('length', $rows);

    $ave_length = round($sum_length / $no_results, 2);

    $sum_chalimus = bamp_sum('chalimus', $rows);

    $ave_chalimus = round($sum_chalimus / $no_results, 2);

    $sum_caligus = bamp_sum('caligus', $rows);

    $ave_caligus = round($sum_caligus / $no_results, 2);

    $sum_total_lice = bamp_sum('total_lice', $rows);

    $ave_total_lice = round($sum_total_lice / $no_results, 2);

    $sum = array(
      array('sum' => 'Total',
        'weight' => $sum_weight,
        'length' => $sum_length,
        'caligus' => $sum_caligus,
        'chalimus' => $sum_chalimus,
        'total_lice' => $sum_total_lice),
      array('sum' => 'Average',
        'weight' => $ave_weight,
        'length' => $ave_length,
        'caligus' => $ave_caligus,
        'chalimus' => $ave_chalimus,
        'total_lice' => $ave_total_lice));

    $header_calc = array(
      t(''),
      t('Weight/kg'),
      t('Length/mm'),
      t('Caligus - Lab'),
      t('Chalimus - Field'),
      t('Total lice'));

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
  
  $no = count($header);
  
  $result = $header[0]['data'];
  $result .= ',';
  $result .= $header[1]['data'];
  
  for ($i = 2; $i < $no; $i++) {
    
  $result .= ',';
  $result .= $header[$i];
  
  }
  
  $form['data'] = array(
    '#type' => 'hidden',
    '#value' => $result,
  );
  
  var_dump($result);
  
  return $form;
}

function bamprepres_menu_callback($species_id=NULL, $trip_id=NULL, $site_id=NULL, $no_results=20) {
  return drupal_get_form('bamprepres_form', $species_id, $trip_id, $site_id, $no_results);
}

function bamprepres_form_submit($form, &$form_state) {
  
$result = $form_state['values']['data'];

$filename = 'data.csv';

drupal_add_http_header('Content-Type', 'text/csv');
drupal_add_http_header('Content-Disposition: attachment', 'filename=' . $filename);
  
//  print($result) . "\r\n";
 
  exit; 
//export_to_csv($data);

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

function bamprepfield_form($form, &$form_state, $species_id, $trip_id, $site_id) {
  drupal_set_title(t('Field results report form'));

  if ($species_id == NULL) {
    $species_message = 'Species: All<br />';
  }
  else {
    $species_message = 'Species: ' . $species_id . '<br />';
  }

  if ($trip_id == NULL) {
    $trip_message = 'Trip: All<br />';
  }
  else {
    $trip_message = 'Trip: ' . $trip_id . '<br />';
  }

  if ($site_id == NULL) {
    $site_message = 'Site: All<br />';
  }
  else {
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
    t('Sample ID'),
    t('Site ID'),
    t('Set number'),
    t('Blind number'),
    t('To Lab'),
    t('Date'),
    t('Pen'),
    t('Input name'),
    t('Route'),
    t('Site number'),
    t('Repetition number'),
    t('Number of fish sampled'),
    t('Crew'),
    t('Species'),
    t('Height/mm'),
    t('Length/mm'),
    t('Weight'),
    t('Lep copepods'),
    t('Cal copepods'),
    t('Total copepods'),
    t('Chalimus a'),
    t('Chalimus b'),
    t('Total chalimus'),
    t('Lep pre-adult male'),
    t('Lep pre-adult female'),
    t('Lep pre-adult unidentified'),
    t('Total lep pre-adult'),
    t('Lep adult male'),
    t('Lep adult female'),
    t('Lep adult gravid'),
    t('Lep adult unidentified'),
    t('Total lep adult'),
    t('Total lep'),
    t('Caligus pre-adult male'),
    t('Caligus pre-adult female'),
    t('Caligus pre-adult unidentified'),
    t('Total caligus pre-adult'),
    t('Caligus adult male'),
    t('Caligus adult female'),
    t('Caligus adult gravid'),
    t('Caligus adult unidentified'),
    t('Total caligus adult'),
    t('Total caligus'),
    t('Total motile'),
    t('Total lice'),
    t('Scar - chalimus'),
    t('Scar - motile'),
    t('Predation marks'),
    t('Hemorrhaging'),
    t('Mate guarding'),
    t('Pinched belly'),
    t('Tissue damage'),
    t('Comments'));

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search details'),
  );

  $select = db_select('bamp_field_results', 'f')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($page_limit);

  if ($species_id == "") {
    $species_id = NULL;
  }

  if ($trip_id == "") {
    $trip_id = NULL;
  }

  if ($site_id == "") {
    $site_id = NULL;
  }

  if ($species_id !== NULL) {
    $select->condition('f.species', $species_id, '=');
  }

  if ($trip_id !== NULL) {
    $select->condition('f.trip_id', $trip_id, '=');
  }

  if ($site_id !== NULL) {
    $select->condition('f.site_name', $site_id, '=');
  }

  $form['form']['details'] = array(
    '#markup' => $species_message . $trip_message . $site_message,
  );

  $select->addField('f', 'id');
  $select->addField('f', 'fish_id');
  $select->addField('f', 'trip_id');
  $select->addField('f', 'sample_id');
  $select->addField('f', 'site_name');
  $select->addField('f', 'set_no');
  $select->addField('f', 'blind_no');
  $select->addField('f', 'to_lab');
  $select->addField('f', 'date');
  $select->addField('f', 'pen');
  $select->addField('f', 'input_name');
  $select->addField('f', 'route');
  $select->addField('f', 'site_no');
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
  $select->addField('f', 'pred_mark');
  $select->addField('f', 'hem');
  $select->addField('f', 'mate_guard');
  $select->addField('f', 'pin_belly');
  $select->addField('f', 'tissue_damage');
  $select->addField('f', 'comments');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
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

    $sum_chalimus = bamp_sum('total_chal', $rows);

    $ave_chalimus = $sum_chalimus / $page_limit;

//    $sum_caligus = bamp_sum('caligus', $rows);
//
//    $ave_caligus = $sum_caligus / $page_limit;

    $sum_total_lice = bamp_sum('total_lice', $rows);

    $ave_total_lice = $sum_total_lice / $page_limit;

    $sum = array(
      array('sum' => 'Total',
//        'caligus' => $sum_caligus,
        'chalimus' => $sum_chalimus,
        'total_lice' => $sum_total_lice),
      array('sum' => 'Average',
//        'caligus' => $ave_caligus,
        'chalimus' => $ave_chalimus,
        'total_lice' => $ave_total_lice));

    $header_calc = array(
      t(''),
//      t('Caligus'),
      t('Chalimus'),
      t('Total lice'));

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

function bamprepfield_menu_callback($species_id=NULL, $trip_id=NULL, $site_id=NULL) {
  return drupal_get_form('bamprepfield_form', $species_id, $trip_id, $site_id);
}

function bampreplab_form($form, &$form_state, $species_id, $trip_id, $lab) {
  drupal_set_title(t('Lab results report form'));

  if ($species_id == NULL) {
    $species_message = 'Species: All<br />';
  }
  else {
    $species_message = 'Species: ' . $species_id . '<br />';
  }

  if ($trip_id == NULL) {
    $trip_message = 'Trip: All<br />';
  }
  else {
    $trip_message = 'Trip: ' . $trip_id . '<br />';
  }

  if ($lab == NULL) {
    $lab_message = 'Lab: All<br />';
  }
  else {
    $lab_message = 'Lab: ' . $lab . '<br />';
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
    t('Lab'),
    t('Date'),
    t('Group ID'),
    t('Species'),
    t('Weight/kg'),
    t('Length/mm'),
    t('Caligus'),
    t('Chalimus'),
    t('Total lice'),
    t('Species'));

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search details'),
  );

  $select = db_select('bamp_lab_results', 'l')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($page_limit);

  if ($species_id == "") {
    $species_id = NULL;
  }

  if ($trip_id == "") {
    $trip_id = NULL;
  }

  if ($lab == "") {
    $lab = NULL;
  }

  if ($species_id !== NULL) {
    $select->condition('l.species', $species_id, '=');
  }

  if ($trip_id !== NULL) {
    $select->condition('l.trip_id', $trip_id, '=');
  }

  if ($lab !== NULL) {
    $select->condition('l.lab', $lab, '=');
  }

  $form['form']['details'] = array(
    '#markup' => $species_message . $trip_message . $lab_message,
  );

  $select->addField('l', 'id');
  $select->addField('l', 'fish_id');
  $select->addField('l', 'trip_id');
  $select->addField('l', 'lab');
  $select->addField('l', 'date');
  $select->addField('l', 'group_id');
  $select->addField('l', 'species');
  $select->addField('l', 'weight');
  $select->addField('l', 'length');
  $select->addField('l', 'caligus');
  $select->addField('l', 'chalimus');
  $select->addField('l', 'total_lice');
  $select->addField('l', 'species');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
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

    $sum_weight = bamp_sum('weight', $rows);

    $ave_weight = round($sum_weight / $page_limit, 2);

    $sum_length = bamp_sum('length', $rows);

    $ave_length = round($sum_length / $page_limit, 2);
    
    $sum_chalimus = bamp_sum('chalimus', $rows);

    $ave_chalimus = $sum_chalimus / $page_limit;

    $sum_caligus = bamp_sum('caligus', $rows);

    $ave_caligus = $sum_caligus / $page_limit;

    $sum_total_lice = bamp_sum('total_lice', $rows);

    $ave_total_lice = $sum_total_lice / $page_limit;

    $sum = array(
      array('sum' => 'Total',
        'weight' => $sum_weight,
        'length' => $sum_length,
        'caligus' => $sum_caligus,
        'chalimus' => $sum_chalimus,
        'total_lice' => $sum_total_lice),
      array('sum' => 'Average',
        'weight' => $ave_weight,
        'length' => $ave_length,
        'caligus' => $ave_caligus,
        'chalimus' => $ave_chalimus,
        'total_lice' => $ave_total_lice));

    $header_calc = array(
      t(''),
      t('Weight/kg'),
      t('Length/mm'),
      t('Caligus'),
      t('Chalimus'),
      t('Total lice'));

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

function bampreplab_menu_callback($species_id=NULL, $trip_id=NULL, $site_id=NULL) {
  return drupal_get_form('bampreplab_form', $species_id, $trip_id, $site_id);
}