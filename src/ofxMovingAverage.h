/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 * This is a port of my Obj-C Moving Average Class on RosettaCode:
 * http://rosettacode.org/wiki/Moving_Average#Objective-C
 *
 */
#pragma once
 
#include <vector>

/// a simple moving average https://en.wikipedia.org/wiki/Moving_average
class ofxMovingAverage {

	public:
 
		/// default window size: 10
		ofxMovingAverage() {
			windowSize = 10;
			sum = 0.0;
			value = 0.0;
			bUpdateValue = false;
		}
		 
		/// specified window size
		ofxMovingAverage(unsigned int windowSize) {
			this->windowSize = windowSize;
			sum = 0.0;
			value = 0.0;
			bUpdateValue = false;
		}
	 
		/// add a new number to the window
		void add(double val) {
			sum += val;
			window.push_back(val);
			if(window.size() > windowSize) { // pop front
				sum -= window.front();
				window.erase(window.begin());
			}
			bUpdateValue = true;
		}
	 
		/// get the average value
		double avg() {
			if(window.size() == 0) {
				return 0; // technically the average is undefined
			}
			if(bUpdateValue) { // avoid unnecessary divisions
				value = sum / window.size();
				bUpdateValue = false;
			}
			return value;
		}
		
		/// operator wrappers, = adds & returns new average
		operator double() {return avg();}
		double operator=(const double &d) {add(d); return avg();}
		double operator+(const double &d) {return d + avg();}
		double operator-(const double &d) {return d - avg();}
		double operator*(const double &d) {return d * avg();}
		double operator/(const double &d) {return d / avg();}
		double operator+=(const double &d) {add(d + avg()); return avg();}
		double operator-=(const double &d) {add(d - avg()); return avg();}
		double operator*=(const double &d) {add(d * avg()); return avg();}
		double operator/=(const double &d) {add(d / avg()); return avg();}
	
		double operator=(const float &f) {add(f); return avg();}
		double operator+(const float &f) {return f + avg();}
		double operator-(const float &f) {return f - avg();}
		double operator*(const float &f) {return f * avg();}
		double operator/(const float &f) {return f / avg();}
		double operator+=(const float &f) {add(f + avg()); return avg();}
		double operator-=(const float &f) {add(f - avg()); return avg();}
		double operator*=(const float &f) {add(f * avg()); return avg();}
		double operator/=(const float &f) {add(f / avg()); return avg();}
	
		double operator=(const int &i) {add(i); return avg();}
		double operator+(const int &i) {return i + avg();}
		double operator-(const int &i) {return i - avg();}
		double operator*(const int &i) {return i * avg();}
		double operator/(const int &i) {return i / avg();}
		double operator+=(const int &i) {add(i + avg()); return avg();}
		double operator-=(const int &i) {add(i - avg()); return avg();}
		double operator*=(const int &i) {add(i * avg()); return avg();}
		double operator/=(const int &i) {add(i / avg()); return avg();}
	
		/// set (resize) the window size
		void setSize(unsigned int windowSize) {
			// make smaller? remove items from the front
			while(window.size() > windowSize) {
				sum -= window.front();
				window.erase(window.begin());
			}
			this->windowSize = windowSize;
			bUpdateValue = true;
		}
	 
		/// get the window size
		unsigned int size() {
			return windowSize;
		}
	 
		/// clear the window and current sum
		void clear() {
			window.clear();
			sum = 0.0;
			value = 0.0;
			bUpdateValue = false;
		}
 
	protected:

		unsigned int windowSize; //< window size
		vector<double> window;   //< current values
		double sum;              //< current value sum
		double value;            //< current averaged value
		bool bUpdateValue;       //< does the value need to be updated?
};
