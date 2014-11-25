<?php

/**
 * @file
 * Response type class definition.
 */

class H5pQuizResponse extends QuizQuestionResponse {

  /**
   * @copydoc QuizQuestionResponse::save()
   */
  public function save() {
    global $user;

    $scaled = h5p_quiz_data_load_score($user->uid,$this->question->nid,$this->question->vid);

    // Something went wrong. Set a score of -1.
    if (!isset($scaled) || !is_numeric($scaled)) {
      $scaled = -1;
    }

    db_insert('h5p_quiz_user_results')
      ->fields(array(
        'question_nid' => $this->question->nid,
        'question_vid' => $this->question->vid,
        'result_id' => $this->rid,
        'score_scaled' => $scaled,
      ))
      ->execute();
  }

  /**
   * @copydoc QuizQuestionResponse::delete()
   */
  public function delete() {
    db_delete('h5p_quiz_user_results')
      ->condition('question_nid', $this->question->nid)
      ->condition('question_vid', $this->question->vid)
      ->condition('result_id', $this->rid)
      ->execute();
  }

  /**
   * @copydoc QuizQuestionResponse::score()
   */
  public function score() {
    $scaled = db_select('h5p_quiz_user_results', 'o')
      ->fields('o', array('score_scaled'))
      ->condition('question_nid', $this->question->nid)
      ->condition('question_vid', $this->question->vid)
      ->condition('result_id', $this->rid)
      ->execute()
      ->fetchField();


    $scaled *= $this->getMaxScore();

    return $scaled;
  }

  /**
   * @copydoc QuizQuestionResponse::getResponse()
   */
  public function getResponse() {
    return 'In H5p package';
  }

  /**
   * Implementation of getReportForm
   *
   * @see QuizQuestionResponse#getReportForm($showpoints, $showfeedback, $allow_scoring)
   */
  public function getReportForm($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    return array(
      '#no_report' => TRUE,
    );
  }
	

  public function getReportFormAnswerFeedback($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    return FALSE;
  }

}
