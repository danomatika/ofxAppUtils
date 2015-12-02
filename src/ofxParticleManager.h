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


///	\class  ofxParticleManager
///	\brief  base class that creates, manages, and destroys particles
class ofxParticleManager {
	public:

		ofxParticleManager(bool autoRemove=true) : bAutoRemove(autoRemove) {}
		virtual ~ofxParticleManager() {
			clear(); // cleanup
		}

	/// \section Particle Control

		/// add a particle to the particle list,
		/// note: the particle will be destroyed by this object
		void addParticle(ofxParticle *particle) {
			if(particle == NULL) {
				ofLogWarning("ofxParticleManager") << "cannot add NULL particle";
				return;
			}
			particles.push_back(particle);
		}
	
		/// pop (remove) the first aka oldest particle
		void popFirstParticle() {
			if(!particles.empty()) {
				particles.erase(particles.begin());
			}
		}
	
		/// pop (remove) the last aka newest particle
		void popLastParticle() {
			if(!particles.empty()) {
				particles.pop_back();
			}
		}

		/// clear (delete) all particles
		void clear() {
			std::vector<ofxParticle*> ::iterator iter;
			for(iter = particles.begin(); iter != particles.end(); ++iter) {
				if((*iter) != NULL) {
					delete (*iter);
				}
			}
			particles.clear();
		}
		
		/// automatically remove (delete) dead particles?
		inline bool getAutoRemove() {return bAutoRemove;}
		void setAutoRemove(bool yesno) {bAutoRemove = yesno;}
    
    /// \section Particle Access
    
        /// get a particle by index, returns NULL if index is out of bounds
        /// note: this pointer will be invalidated if the particle is destroyed
        ofxParticle* getParticle(unsigned int index) {
            if(index < particles.size()) {
                return particles.at(index);
            }
            return NULL;
        }
    
        /// get the first particle, returns NULL if list is empty
        /// note: this pointer will be invalidated if the particle is destroyed
        ofxParticle* getFirstParticle() {
            if(!particles.empty()) {
                return particles.front();
            }
            return NULL;
        }
    
        /// get the last particle, returns NULL if list is empty
        /// note: this pointer will be invalidated if the particle is destroyed
        ofxParticle* getLastParticle() {
            if(!particles.empty()) {
                return particles.back();
            }
            return NULL;
        }
	
		/// get access to the particles vector
		vector<ofxParticle*>& getParticles() {return particles;};
		
	/// \section Update & Draw

		/// update all particles
		virtual void update() {
			std::vector<ofxParticle*> ::iterator iter;
			for(iter = particles.begin(); iter != particles.end();) {
				// remove particle if it's NULL
				if((*iter) == NULL) {
					ofLogWarning("ofxParticleManager") << "update(): removing NULL particle";
					iter = particles.erase(iter);
				}
				else {
					// auto remove dead particles?
					if(bAutoRemove && !(*iter)->isAlive()) {
						delete (*iter);
						iter = particles.erase(iter);
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
			for(iter = particles.begin(); iter != particles.end();){
				// remove particle if it's NULL
				if((*iter) == NULL) {
					ofLogWarning("ofxParticleManager") << "draw(): removing NULL particle";
					iter = particles.erase(iter);
				}
				else {
					(*iter)->draw();
					++iter; // increment iter
				}
			}
		}
		
	/// \section Util
		
		/// get the number of particles
		unsigned int size() {
			return particles.size();
		}
		
		/// are there any particles at all?
		bool empty() {
			return particles.empty();
		}

	protected:

		bool bAutoRemove; //< automatically remove dead particles?
		std::vector<ofxParticle*> particles; //< current particles
};
