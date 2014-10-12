<?php
/**
*version             1.0
*@author             sanshi
*QQ:                                 35047205
*MSN:                                 sanshi0815@tom.com
*Create              2005/6/18
*******************************************************
*@param   string     $srcFile    
*@param   string     $dstFile    
*@param   string     $fileType   
*@param   string     $im         
*@param   array      $imgType    
*/
class Thumbnail
{
        var $srcFile;             
        var $dstFile;             
        var $fileType;            
        var $im;                  
        var $imgType=array("jpg", 
                                           "JPG", 
                                           "gif",
                                           "png",
                                           "bmp");
        /**

        *@param  string $fileName     
        *@return boolean              
        */
        function findType($fileName)
        {
                $type=pathinfo($fileName);
                $var=$type['extension'];
                for($i=0;$i<=count($this->imgType);$i++)
                {
                        if(Strcmp($this->imgType[$i],$var)==0)
                        {
                                $this->fileType=$var;
                                return true;
                        }
                }
                return false;
        }
        /**
        *@param    $fileType     
        *@return   resource      
        */
        function loadImg($fileType)
        {
                $type=$this->isNull($fileType);
                switch($type)
                {
                        case "jpg":
                                $im=ImageCreateFromjpeg($this->srcFile);
                                break;
                        case "JPG":
                                $im=ImageCreateFromjpeg($this->srcFile);
                                break;
                        case "gif":
                                $im=ImageCreateFromGIF($this->srcFile);
                                break;
                        case "png":
                                $im=imagecreatefrompng($this->srcFile);
                                break;
                        case "bmp":
                                $im=imagecreatefromwbmp($this->srcFile);
                                break;
                        default:
                                $im=0;
                                echo "not you input file type!<br>";
                                break;
                }
                $this->im=$im;
                return $im;
        }
        /**

        */
        function isNull($var)
    {
       if(!isset($var)||empty($var))
        {
            echo "！<br>";
            exit(0);
         }
         return $var;
     }
        /**
        

        *@param string  srcFile     
        *@param String  dstFile      
        */
        function setParam($srcFile,$dstFile)
        {
                $this->srcFile=$this->isNull($srcFile);
                $this->dstFile=$this->isNull($dstFile);
                if(!$this->findType($srcFile))
                {
                        echo "file type error!asdfas";
                }
                if(!$this->loadImg($this->fileType))
                {
                        echo "open ".$this->srcFile."error!<br>";
                }
        }
        /**
        *@param    resource im     
        *@return   int      width  
        */
        function getImgWidth($im)
        {
                $im=$this->isNull($im);
                $width=imagesx($im);
                return $width;
        }
        /**
        *@param    resource im      
        *@return   int      height  
        */
        function getImgHeight($im)
        {
                $im=$this->isNull($im);
                $height=imagesy($im);
                return $height;
        }
        /**
        *@param     resource im     
        *@param     int      scale   
        *@param     boolean  page    
        */
        function createImg($im,$scale,$page)
        {
                $im=$this->isNull($im);
                $scale=$this->isNull($scale);
                $srcW=$this->getImgWidth($im);
                $srcH=$this->getImgHeight($im);
                $detW=round($srcW*$scale/100);
                $detH=round($srcH*$scale/100);
                //$om=ImageCreate($detW,$detH);
                $om=imagecreatetruecolor($detW,$detH);
                //ImageCopyResized($om,$im,0,0,0,0,$detW,$detH,$srcW,$srcH);
                imagecopyresampled($om,$im,0,0,0,0,$detW,$detH,$srcW,$srcH);
                $this->showImg($om,$this->fileType,$page);

        }
                /**
        *@param     resource im      
        *@param     int      scale   
        *@param     boolean  page   
        */
        function createNewImg($im,$width,$height,$page)
        {
                $im=$this->isNull($im);
                //$scale=$this->isNull($scale);
                $srcW=$this->getImgWidth($im);
                $srcH=$this->getImgHeight($im);
                $detW=$this->isNull($width);
                $detH=$this->isNull($height);
                //$om=ImageCreate($detW,$detH);
                $om=imagecreatetruecolor($detW,$detH);
                //ImageCopyResized($om,$im,0,0,0,0,$detW,$detH,$srcW,$srcH);
                imagecopyresampled($om,$im,0,0,0,0,$detW,$detH,$srcW,$srcH);
                $this->showImg($om,$this->fileType,$page);

        }
        /**
        *@param boolean   boolean  
        */
        function inputError($boolean)
        {
                if(!$boolean)
                {
                        echo "img input error!<br>";
                }
        }
        /**
        *@param  resource     $om       
        *@param  String       $type     
        *@param  boolean      $page     
        */
        function showImg($om,$type,$page)
        {
                $om=$this->isNull($om);
                $type=$this->isNull($type);
                switch($type)
                {
                        case "jpg":
                                if($page)
                                {
                                  $suc=imagejpeg($om);
                                  $this->inputError($suc);
                                }else{
                                  $suc=imagejpeg($om,$this->dstFile);
                                  $this->inputError($suc);
                                }
                                break;
                        case "JPG":
                                if($page)
                                {
                                  $suc=imagejpeg($om);
                                  $this->inputError($suc);
                                }else{
                                  $suc=imagejpeg($om,$this->dstFile);
                                  $this->inputError($suc);
                                }
                                break;
                        case "gif":
                                if($page)
                                {
                                  $suc=imagegif($om);
                                  $this->inputError($suc);
                                }else{
                                  $suc=imagegif($om,$this->dstFile);
                                  $this->inputError($suc);
                                }
                                break;
                        case "png":
                                if($page)
                                {
                                  $suc=imagepng($om);
                                  $this->inputError($suc);
                                }else{
                                  $suc=imagepng($om,$this->dstFile);
                                  $this->inputError($suc);
                                }
                                break;
                        case "bmp":
                                if($page)
                                {
                                  $suc=imagewbmp($om);
                                  $this->inputError($suc);
                                }else{
                                  $suc=imagewbmp($om,$this->dstFile);
                                  $this->inputError($suc);
                                }
                                break;
                        default:
                                echo "not you input file type!<br>";
                                break;
                }
        }
}
/*
include('thumbnail.cls.php');

$file=new Thumbnail();
$file->setParam("img/logo.jpg","img/logo1.jpg");//设置源文件，跟生成文件

//$file->createImg($file->im,50,true);//按比例生成图象，比例为200%，在页面上显示
$file->createImg($file->im,50,false);//按比例生成图象，比例为200%，生成图片保存到上面设置的名字和路径
//$file->createNewImg($file->im,100,100,true);//按照自己设计的长宽生成图象，保存或者显示在页面上
*/
?>