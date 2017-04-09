/* Copyright 2017 Kristofer Björnson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @package TBTKcalc
 *  @file RayTracer.h
 *  @brief Creates figures of properties using ray tracing.
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_RAY_TRACER
#define COM_DAFER45_TBTK_RAY_TRACER

#include "Density.h"
#include "LDOS.h"
#include "Magnetization.h"
#include "Model.h"
#include "WaveFunction.h"
#include "TBTKMacros.h"
#include "Vector3d.h"

#include <functional>
#include <initializer_list>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace TBTK{

class RayTracer{
public:
	/** COnstructor. */
	RayTracer();

	/** Destructor. */
	~RayTracer();

	/** Set camera position. */
	void setCameraPosition(const Vector3d &cameraPosition);

	/** Set camera position. */
	void setCameraPosition(std::initializer_list<double> cameraPosition);

	/** Set camera focus. */
	void setFocus(const Vector3d &focus);

	/** Set camera focus. */
	void setFocus(std::initializer_list<double> focus);

	/** Set up direction. */
	void setUp(const Vector3d &up);

	/** Set up direction. */
	void setUp(std::initializer_list<double> up);

	/** Set viewport width. */
	void setWidth(unsigned int width);

	/** Set viewport height. */
	void setHeight(unsigned int height);

	/** Set state radius. */
	void setStateRadius(double stateRadius);

	/** Set trace depth. */
	void setTraceDepth(unsigned int traceDepth);

	/** Get trace depth. */
	unsigned int getTraceDepth() const;

	/** Plot Density. */
	void plot(const Model& model, const Property::Density &density);

	/** Plot Magnetization. */
	void plot(
		const Model &model,
		const Property::Magnetization &magnetization
	);

	/** Plot Magnetization. */
	void plot(
		const Model &model,
		const Property::WaveFunction &waveFunction,
		unsigned int state
	);

	/** Interactive. */
	void interactivePlot(
		const Model &model,
		const Property::LDOS &ldos,
		double sigma = 0,
		unsigned int windowSize = 51
	);
private:
	/** Class for encoding RGB colors. */
	class Color{
	public:
		double r, g, b;
	};

	/** Class for describing materials. */
	class Material{
	public:
		/** Constructor. */
		Material();

		/** Color. */
		Color color;

		/** Light properties. */
		double ambient, diffusive, emissive, specular;

		/** Default material parameters. */
		static constexpr double DEFAULT_AMBIENT = 1;
		static constexpr double DEFAULT_DIFFUSIVE = 0.5;
		static constexpr double DEFAULT_EMISSIVE = 0;
		static constexpr double DEFAULT_SPECULAR = 0.1;
	};

	/** */
	class RenderContext{
	public:
		/** Constructor. */
		RenderContext();

		/** Destructor. */
		~RenderContext();

		/** Set camera position. */
		void setCameraPosition(const Vector3d &cameraPosition);

		/** Set camera position. */
		void setCameraPosition(std::initializer_list<double> cameraPosition);

		/** Get cameraPosition. */
		const Vector3d& getCameraPosition() const;

		/** Set focus. */
		void setFocus(const Vector3d &focus);

		/** Set camera focus. */
		void setFocus(std::initializer_list<double> focus);

		/** Get focus point. */
		const Vector3d& getFocus() const;

		/** Set up direction. */
		void setUp(const Vector3d &up);

		/** Set up direction. */
		void setUp(std::initializer_list<double> up);

		/** Get up direction. */
		const Vector3d& getUp() const;

		/** Set viewport width. */
		void setWidth(unsigned int width);

		/** Get viewport width. */
		unsigned int getWidth() const;

		/** Set viewport height. */
		void setHeight(unsigned int height);

		/** Get viewport height. */
		unsigned int getHeight() const;

		/** Set state radius. */
		void setStateRadius(double stateRadius);

		/** Get state radius. */
		double getStateRadius() const;

		/** Set ray depth. */
		void setTraceDepth(unsigned int traceDepth);

		/*** Get ray depth. */
		unsigned int getTraceDepth() const;
	private:
		/** Camera position. */
		Vector3d cameraPosition;

		/** Focus point. */
		Vector3d focus;

		/** Up direction. */
		Vector3d up;

		/** Viewport width. */
		double width;

		/** Viewport height. */
		double height;

		/** State radius. */
		double stateRadius;

		/** Maximum number of times a ray will be traced after having
		 *  been deflected. */
		unsigned int traceDepth;
	};

	RenderContext renderContext;

	/** */
	class HitDescriptor{
	public:
		/** Constructor. */
		HitDescriptor(const RenderContext &renderContext);

		/** Copy construtor. */
		HitDescriptor(const HitDescriptor &hitDescriptor);

		/** Move constructor. */
		HitDescriptor(HitDescriptor &&hitDescriptor);

		/** Destructor. */
		~HitDescriptor();

		/** Assignment operator. */
		HitDescriptor& operator=(const HitDescriptor &rhs);

		/** Move assignment operator. */
		HitDescriptor& operator=(HitDescriptor &&rhs);

		/** Set ray source. */
		void setRaySource(const Vector3d &raySource);

		/** Get ray source. */
		const Vector3d& getRaySource() const;

		/** Set ray direction. */
		void setRayDirection(const Vector3d &rayDirection);

		/** Get ray direction. */
		const Vector3d& getRayDirection() const;

		/** Set index. */
		void setIndex(const Index &index);

		/** Get index. */
		const Index& getIndex() const;

		/** Set coordinate. */
		void setCoordinate(const Vector3d coordainte);

		/** Get coordinate. */
		const Vector3d& getCoordinate() const;

		/** Get directionFromObject. */
		const Vector3d& getDirectionFromObject();

		/** Get impact position. */
		const Vector3d& getImpactPosition();
	private:
		/** Render context. */
		const RenderContext *renderContext;

		/** Ray source. */
		Vector3d raySource;

		/** Ray direction. */
		Vector3d rayDirection;

		/** Index that was hit. */
		Index index;

		/** Coordinate. */
		Vector3d coordinate;

		/** Direction from object. */
		Vector3d *directionFromObject;

		/** Impact position. */
		Vector3d *impactPosition;
	};

	/** Perform ray tracing. */
	void render(
		const IndexDescriptor &indexDescriptor,
		const Model &model,
		std::function<Material(HitDescriptor &hitDescriptor)> &&lambdaColorPicker,
		std::function<void(cv::Mat &canvas, const Index &index)> &&lambdaInteractive = {}
	);

	Color trace(
		const std::vector<Vector3d> &coordinates,
		const Vector3d &raySource,
		const Vector3d &rayDirection,
		const IndexTree &indexTree,
		std::vector<HitDescriptor> &hitDescriptors,
		std::function<Material(HitDescriptor &hitDescriptor)> lambdaColorPicker,
		unsigned int depth = 0
	);

	/** Event handler for the interactive mode. */
	class EventHandler{
	public:
		/** Try to lock EventHandler. Returns true if successful. */
		static bool lock(
			RayTracer *owner,
			std::function<void(
				int event,
				int x,
				int y,
				int flags,
				void *userData
			)> &&lambdaOnMouseChange
		);

		/** Unlock EventHandler. */
		static bool unlock(const RayTracer *owner);

		/** On mouse change callback. */
		static void onMouseChange(
			int event,
			int x,
			int y,
			int flags,
			void *userdata
		);
	private:
		/** Flag indicating whether the EventHandler is locked. */
		static bool isLocked;

		/** Owner of the lock. */
		static RayTracer *owner;

		/** On mouse change lambda function. */
		static std::function<void(int event, int x, int y, int flags, void *userData)> &&lambdaOnMouseChange;
	};
};

inline void RayTracer::setCameraPosition(const Vector3d &cameraPosition){
	renderContext.setCameraPosition(cameraPosition);
}

inline void RayTracer::setCameraPosition(
	std::initializer_list<double> cameraPosition
){
	renderContext.setCameraPosition(cameraPosition);
}

inline void RayTracer::setFocus(const Vector3d &focus){
	renderContext.setFocus(focus);
}

inline void RayTracer::setFocus(std::initializer_list<double> focus){
	renderContext.setFocus(focus);
}

inline void RayTracer::setUp(const Vector3d &up){
	renderContext.setUp(up);
}

inline void RayTracer::setWidth(unsigned int width){
	renderContext.setWidth(width);
}

inline void RayTracer::setHeight(unsigned int height){
	renderContext.setHeight(height);
}

inline void RayTracer::setUp(std::initializer_list<double> up){
	renderContext.setUp(up);
}

inline void RayTracer::setStateRadius(double stateRadius){
	renderContext.setStateRadius(stateRadius);
}

inline void RayTracer::setTraceDepth(unsigned int traceDepth){
	renderContext.setTraceDepth(traceDepth);
}

inline unsigned int RayTracer::getTraceDepth() const{
	return renderContext.getTraceDepth();
}

inline RayTracer::Material::Material(){
	color.r = 0;
	color.g = 0;
	color.b = 0;
	ambient = DEFAULT_AMBIENT;
	diffusive = DEFAULT_AMBIENT;
	emissive = DEFAULT_EMISSIVE;
	specular = DEFAULT_SPECULAR;
}

inline void RayTracer::RenderContext::setCameraPosition(
	const Vector3d &cameraPosition
){
	this->cameraPosition = cameraPosition;
}

inline void RayTracer::RenderContext::setCameraPosition(
	std::initializer_list<double> cameraPosition
){
	TBTKAssert(
		cameraPosition.size() == 3,
		"RayTracer::setCameraPosition()",
		"Camera position can only have three coordinates.",
		""
	);

	this->cameraPosition.x = *(cameraPosition.begin() + 0);
	this->cameraPosition.y = *(cameraPosition.begin() + 1);
	this->cameraPosition.z = *(cameraPosition.begin() + 2);
}

inline const Vector3d& RayTracer::RenderContext::getCameraPosition() const{
	return cameraPosition;
}

inline void RayTracer::RenderContext::setFocus(const Vector3d &focus){
	this->focus = focus;
}

inline void RayTracer::RenderContext::setFocus(
	std::initializer_list<double> focus
){
	TBTKAssert(
		focus.size() == 3,
		"RayTracer::setFocus()",
		"Focus can only have three coordinates.",
		""
	);

	this->focus.x = *(focus.begin() + 0);
	this->focus.y = *(focus.begin() + 1);
	this->focus.z = *(focus.begin() + 2);
}

inline const Vector3d& RayTracer::RenderContext::getFocus() const{
	return focus;
}

inline void RayTracer::RenderContext::setUp(const Vector3d &up){
	this->up = up;
}

inline void RayTracer::RenderContext::setUp(
	std::initializer_list<double> up
){
	TBTKAssert(
		up.size() == 3,
		"RayTracer::setCameraPosition()",
		"Camera position can only have three coordinates.",
		""
	);

	this->up.x = *(up.begin() + 0);
	this->up.y = *(up.begin() + 1);
	this->up.z = *(up.begin() + 2);
}

inline const Vector3d& RayTracer::RenderContext::getUp() const{
	return up;
}

inline void RayTracer::RenderContext::setWidth(unsigned int width){
	this->width = width;
}

inline unsigned int RayTracer::RenderContext::getWidth() const{
	return width;
}

inline void RayTracer::RenderContext::setHeight(unsigned int height){
	this->height = height;
}

inline unsigned int RayTracer::RenderContext::getHeight() const{
	return height;
}

inline void RayTracer::RenderContext::setStateRadius(double stateRadius){
	this->stateRadius = stateRadius;
}

inline double RayTracer::RenderContext::getStateRadius() const{
	return stateRadius;
}

inline void RayTracer::RenderContext::setTraceDepth(unsigned int traceDepth){
	this->traceDepth = traceDepth;
}

inline unsigned int RayTracer::RenderContext::getTraceDepth() const{
	return traceDepth;
}

inline void RayTracer::HitDescriptor::setRaySource(
	const Vector3d &raySource
){
	this->raySource = raySource;
}

inline const Vector3d& RayTracer::HitDescriptor::getRaySource() const{
	return this->raySource;
}

inline void RayTracer::HitDescriptor::setRayDirection(
	const Vector3d &rayDirection
){
	this->rayDirection = rayDirection;
}

inline const Vector3d& RayTracer::HitDescriptor::getRayDirection() const{
	return this->rayDirection;
}

inline void RayTracer::HitDescriptor::setIndex(const Index &index){
	this->index = index;
}

inline const Index& RayTracer::HitDescriptor::getIndex() const{
	return index;
}

inline void RayTracer::HitDescriptor::setCoordinate(const Vector3d coordinate){
	this->coordinate = coordinate;
}

inline const Vector3d& RayTracer::HitDescriptor::getCoordinate() const{
	return coordinate;
}

inline bool RayTracer::EventHandler::lock(
	RayTracer *owner,
	std::function<void(
		int event,
		int x,
		int y,
		int flags,
		void *userData
	)> &&lambdaOnMouseChange
){
	if(isLocked){
		return false;
	}
	else{
		isLocked = true;
		EventHandler::owner = owner;
		EventHandler::lambdaOnMouseChange = lambdaOnMouseChange;
		return true;
	}
}

inline bool RayTracer::EventHandler::unlock(const RayTracer *owner){
	if(EventHandler::owner == owner){
		EventHandler::owner = nullptr;
		EventHandler::isLocked = false;
		EventHandler::lambdaOnMouseChange = nullptr;
		return true;
	}
	else{
		return false;
	}
}

inline void RayTracer::EventHandler::onMouseChange(
	int event,
	int x,
	int y,
	int flags,
	void *userData
){
	if(lambdaOnMouseChange){
		lambdaOnMouseChange(event, x, y, flags, userData);
	}
	else{
		TBTKExit(
			"RayTracer::EventHandler::onMouseChange()",
			"lambdaOnMouseChange is nullptr.",
			"This should never happen, contact the developer."
		);
	}
}

};	//End namespace TBTK

#endif