<?php
/* @var $this SiteController */

$this->pageTitle=Yii::app()->name;
?>

<h1>Goals:</h1>

<div class="walter_goal">
<ul> 
<li>Read the book of yii</li>
<li>Write tutorial of yii</li>
<li>Implement the  main function of PIMS</li>
</ul>


</div>

<div>
<h3>Todo list</h3>
<ol>
	<li> Article CRUDLS on a single page
	<li> Task management
	<li> Provide Restful API
	<li> Integrate with WebRTC
</ol>
</div>
<p>You may change the content of this page by modifying the following two files:</p>
<ul>
	<li>View file: <code><?php echo __FILE__; ?></code></li>
	<li>Layout file: <code><?php echo $this->getLayoutFile('main'); ?></code></li>
</ul>

<div class="annotation">
<table>
 <thead>
  <tr>
<th width="%3">Keep?</th>
<th>#</th>
<th>User</th>
<th>Action</th>
<th>Content</th>
<th>Time</th>
</tr>
</thead>
<tr>
<td width="%3"><input type="checkbox" name="enabled" value="true" checked="true"> </td>
<td>1</td>
<td>Alice</td>
<td>Pointer</td>
<td></td>
<td>2014-05-27 09:35</td>
</tr>
<tr>
<td><input type="checkbox" name="enabled" value="true"  checked="true"> </td>
<td>2</td>
<td>Bob</td>
<td>Text</td>
<td>I think it does not make sense</td>
<td>2014-05-27 09:46</td>
</tr>

<tr>
<td><input type="checkbox" name="enabled" value="true"> </td>
<td>3</td>
<td>Calvin</td>
<td>Rectangle</td>
<td></td>
<td>2014-05-27 09:47</td>
</tr>
<tr>
<td><input type="checkbox" name="enabled" value="true"> </td>
<td>4</td>
<td>David</td>
<td>Text</td>
<td>The step is very important</td>
<td>2014-05-27 09:57</td>
</tr>
</table>
</div>
<!--
Pointer
Text
Line
Rectangle
Highlighter
Eraser
-->