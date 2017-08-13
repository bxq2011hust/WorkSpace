package com.neptune.test;

import com.neptune.api.FacialPoint;
import com.neptune.api.Verify;
import java.io.*;
import java.util.Arrays;
import java.nio.file.Files;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.util.List;
import java.util.Formatter; 
/**
 * Created by taozhiheng on 16-11-28.
 */
public class FaceVerifyTest {

	public static int width;

	public static int height;

    public static byte[] readByteFile(String path) throws Exception{
		File fi = new File(path);
		BufferedImage originalImage=ImageIO.read(fi);
		width = originalImage.getWidth();
		height = originalImage.getHeight();
		byte[] pixels = ((DataBufferByte) originalImage.getRaster().getDataBuffer()).getData();
        System.out.println(pixels.length);
		return pixels;
    }

    public static void main(String[] args) throws Exception
    {
		System.out.println("args length = " + args.length);
		if(args.length != 2){
			System.out.println("wrong input");
			return;
		}

        //System.load("/usr/lib/x86_64-linux-gnu/libprotobuf.so");
        System.load("/home/bxq/TestVerify/libcaffe.so.1.0.0-rc3");
        System.load("/home/bxq/TestVerify/MTCNN_Caffe/build/lib/libget_fpoint.so");
        FacialPoint.init("/home/bxq/TestVerify/MTCNN_Caffe/examples/MTmodel/");
		Verify.init("faceverifytest");

		// String path1 = args[0];
		// String path2 = args[1];
        StringBuffer sbuf = new StringBuffer("");
        FileReader reader = new FileReader(args[0]);
        BufferedReader br = new BufferedReader(reader);
       String nameListFile = null;
	   
      while((nameListFile=br.readLine()) != null)
	  {
        FileReader readerImg1 = new FileReader(nameListFile);
        BufferedReader brImg1 = new BufferedReader(readerImg1);
        String imgName1 = null; 
		int total = 0;
		int i=0;
		String resultName = args[0].substring(0,args[0].lastIndexOf("/")) +nameListFile.substring(nameListFile.lastIndexOf("/"),nameListFile.length()-4)+"_result.txt";
		//String resultName = args[0].substring(0,args[0].length()-4) +nameListFile.substring(nameListFile.lastIndexOf("/"),nameListFile.length()-4)+"_result.txt";
		Formatter fformat =  new Formatter(new PrintStream(resultName));
		while((imgName1=brImg1.readLine()) != null)
		{
			total++;
			i++;
			int j=0;
			String path1 = args[1]+nameListFile.substring(nameListFile.lastIndexOf("/"),nameListFile.length()-4)+"/" + imgName1;
        	FileReader readerImg2 = new FileReader(nameListFile);
        	BufferedReader brImg2 = new BufferedReader(readerImg2);
        	String imgName2 = null; 
			while((imgName2=brImg2.readLine()) != null)
			{
				j++;
				if(j<=i)
				{
					fformat.format("0.00\t");
					continue;
				}
				String path2 = args[1]+nameListFile.substring(nameListFile.lastIndexOf("/"),nameListFile.length()-4)+"/" + imgName2;
				//System.out.println(path1+ " -- " + path2);
				int width1, height1, width2, height2;
			
				byte[] image1 = readByteFile(path1);
				width1 = width; height1 = height;
				float[] points1 = FacialPoint.getPoints(image1, width, height);
				if(points1 == null){
					// System.out.printf("0\t");
					fformat.format("0.00\t"); 
					continue;
				}

				byte[] image2 = readByteFile(path2);
				width2 = width; height2 = height;
				float[] points2 = FacialPoint.getPoints(image2, width, height);
				
				if(points2 == null){
					// System.out.printf("0\t");
					fformat.format("0.00\t");  
					continue;
				}

				double dis = Verify.verify(image1, width1, height1, points1, image2, width2, height2, points2);
				// System.out.printf("%.4f\t", dis);
				fformat.format("%.2f\t", dis); 
				
			}
			fformat.format("\n"); 
			System.out.println(total);
			brImg2.close();
			readerImg2.close();
			
		}
        brImg1.close();
        readerImg1.close();
    } 
	br.close();
    reader.close();

	//	System.out.println("dis = " + dis);
	FacialPoint.cleanup();
    }
}
