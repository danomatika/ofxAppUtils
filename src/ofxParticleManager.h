/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#pragma once

#include "ofxParticle.h"

/**
	\class  ofxParticleManager
	\brief  base class that creates, manages, and destroys particles
**/
class ofxParticleManager {
	public:

		ofxParticleManager(bool autoRemove=true) : bAutoRemove(autoRemove) {}
		virtual ~ofxParticleManager() {
			clear();    // cleanup
		}

	/// \section Particle Conctrol

		/// add a particle to the particle list,
		/// note: the particle will be destroyed by this object
		void addParticle(ofxParticle* particle) {
			if(particle == NULL) {
				ofLogWarning("ofxParticleManager") << "cannot add NULL particle";
				return;
			}
			particleList.push_back(particle);
		}
		
		void popOldestParticle() {
			if(!particleList.empty()) {
				particleList.erase(particleList.begin());
			}
		}
		
		void popNewestParticle() {
			if(!particleList.empty()) {
				particleList.pop_back();
			}
		}

		/// clear (delete) all particles in the particle list
		void clear() {
			std::vector<ofxParticle*> ::iterator iter;
			for(iter = particleList.begin(); iter != particleList.end(); ++iter) {
				if((*iter) != NULL) {
					delete (*iter);
				}
			}
			particleList.clear();
		}
		
		/// automatically remove (delete) dead particles?
		inline bool getAutoRemove() {return bAutoRemove;}
		void setAutoRemove(bool yesno) {bAutoRemove = yesno;}
		
	/// \section Update & Draw

		/// update all particles
		virtual void update() {
			std::vector<ofxParticle*> ::iterator iter;
			for(iter = particleList.begin(); iter != particleList.end();) {
				// remove particle if it's NULL
				if((*iter) == NULL) {
					ofLogWarning("ofxParticleManager") << "update(): removing NULL particle";
					iter = particleList.erase(iter);
				}
				else {
					// auto remove dead particles?
					if(bAutoRemove && !(*iter)->isAlive()) {
						delete (*iter);
						iter = particleList.erase(iter);
					}
					else {
						(*iter)->update();
						++iter; // increment iter
					}
				}
			}

		}

		/// draw all the particles
		virtual void draw() {
			std::vector<ofxParticle*> ::iterator iter;
			for(iter = particleList.begin(); iter != particleList.end();){
				// remove particle if it's NULL
				if((*iter) == NULL) {
					ofLogWarning("ofxParticleManager") << "draw(): removing NULL particle";
					iter = particleList.erase(iter);
				}
				else {
					(*iter)->draw();
					++iter; // increment iter
				}
			}
		}
		
			/// \section Util
		
		// get the number of particles
		unsigned int size() {
			return particleList.size();
		}
		
		// are there any particles at all?
		bool empty() {
			return particleList.empty();
		}

	protected:

		bool bAutoRemove;   ///< automatically remove dead particles?

		std::vector<ofxParticle*> particleList;    ///< current particles
};
