/************************************************************************/
/*                                                                      */
/* This file is part of VDrift.                                         */
/*                                                                      */
/* VDrift is free software: you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* VDrift is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with VDrift.  If not, see <http://www.gnu.org/licenses/>.      */
/*                                                                      */
/************************************************************************/

#ifndef _GAME_H
#define _GAME_H

#include "window.h"
#include "graphics.h"
#include "eventsystem.h"
#include "settings.h"
#include "pathmanager.h"
#include "track.h"
#include "mathvector.h"
#include "quaternion.h"
#include "font.h"
#include "text_draw.h"
#include "gui/gui.h"
#include "car.h"
#include "dynamicsworld.h"
#include "dynamicsdraw.h"
#include "carcontrolmap_local.h"
#include "hud.h"
#include "inputgraph.h"
#include "sound.h"
#include "camera.h"
#include "camera_free.h"
#include "trackmap.h"
#include "loadingscreen.h"
#include "timer.h"
#include "replay.h"
#include "forcefeedback.h"
#include "particle.h"
#include "ai/ai.h"
#include "quickmp.h"
#include "contentmanager.h"
#include "http.h"
#include "gl3v/stringidmap.h"
#include "updatemanager.h"
#include "game_downloader.h"

#include <ostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <memory>

class GAME
{
friend class GAME_DOWNLOADER;
public:
	GAME(std::ostream & info_out, std::ostream & error_out);

	~GAME();

	void Start(std::list <std::string> & args);

private:
	void End();

	float TickPeriod() const {return timestep;}

	void MainLoop();

	bool ParseArguments(std::list <std::string> & args);

	void InitCoreSubsystems();

	void InitThreading();

	bool InitSound();

	bool InitGUI(const std::string & pagename);

	void Test();

	void Tick(float dt);

	void Draw();

	void AdvanceGameLogic();

	void UpdateCar(CAR & car, double dt);

	void UpdateDriftScore(CAR & car, double dt);

	void UpdateCarInputs(CAR & car);

	void UpdateTimer();

	///< Check eventsystem state and update GUI
	void ProcessGUIInputs();

	void ProcessGameInputs();

	void UpdateStartList(unsigned i, const std::string & value);

	bool NewGame(bool playreplay=false, bool opponents=false, int num_laps=0);

	///< carfile is a string containing an entire .car file (e.g. XS.car) and is used instead of reading from disk.  this is optional
	bool LoadCar(
		const std::string & carname,
		const std::string & carpaint,
		const MATHVECTOR <float, 3> & carcolorhsv,
		const MATHVECTOR <float, 3> & start_position,
		const QUATERNION <float> & start_orientation,
		bool islocal, bool isai,
		const std::string & carfile="");

	bool LoadTrack(const std::string & trackname);

	void LoadGarage();

	void SetGarageCar();

	void SetCarColor();

	bool LoadFonts();

	void CalculateFPS();

	void PopulateValueLists(std::map<std::string, std::list <std::pair<std::string,std::string> > > & valuelists);

	void PopulateReplayList(std::list <std::pair <std::string, std::string> > & replaylist);

	void PopulateCarPaintList(const std::string & carname, std::list <std::pair <std::string, std::string> > & carpaintlist);

	void UpdateTrackMap();

	void ShowHUD(bool value);

	void LoadingScreen(float progress, float max, bool drawGui, const std::string & optionalText, float x, float y);

	void ProcessNewSettings();

	/// Look for keyboard, mouse, joystick input, assign local car controls.
	bool AssignControl();

	void LoadControlsIntoGUIPage();

	void LoadControlsIntoGUI();

	void UpdateForceFeedback(float dt);

	void UpdateParticleSystems(float dt);

	void AddTireSmokeParticles(float dt, CAR & car);

	std::string GetReplayRecordingFilename();

	void BeginDraw();

	void FinishDraw();

	void BeginStartingUp();

	void DoneStartingUp();

	bool LastStartWasSuccessful() const;

	bool Download(const std::string & file);

	bool Download(const std::vector <std::string> & files);

	// game actions
	void QuitGame();
	void LeaveGame();
	void StartPractice();
	void StartRace();
	void ReturnToGame();
	void RestartGame();
	void StartReplay();
	void HandleOnlineClicked();
	void StartCheckForUpdates();
	void StartCarManager();
	void CarManagerNext();
	void CarManagerPrev();
	void ApplyCarUpdate();
	void StartTrackManager();
	void TrackManagerNext();
	void TrackManagerPrev();
	void ApplyTrackUpdate();
	void EditControl();
	void CancelControl();
	void DeleteControl();
	void SetButtonControl();
	void SetAnalogControl();
	void LoadControls();
	void SaveControls();
	void SyncOptions();
	void SyncSettings();
	void EditFirstCar();
	void EditLastCar();

	void SetCarName(const std::string & value);
	void SetCarPaint(const std::string & value);
	void SetCarColor(const std::string & value);
	void SetCarColorHue(const std::string & value);
	void SetCarColorSat(const std::string & value);
	void SetCarColorVal(const std::string & value);
	void SetCarAIType(const std::string & value);
	void SetCarAILevel(const std::string & value);
	void SetCarsNum(const std::string & value);
	void SetTrackImage(const std::string & value);
	void SetControl(const std::string & value);

	void BindActionsToGUI();
	void RegisterActions();
	void InitActionMap(std::map<std::string, Slot0*> & actionmap);

	Slot1<const std::string &> set_car_name;
	Slot1<const std::string &> set_car_paint;
	Slot1<const std::string &> set_car_color_hue;
	Slot1<const std::string &> set_car_color_sat;
	Slot1<const std::string &> set_car_color_val;
	Slot1<const std::string &> set_car_ai_type;
	Slot1<const std::string &> set_car_ai_level;
	Slot1<const std::string &> set_cars_num;
	Slot1<const std::string &> set_track_image;
	Slot1<const std::string &> set_control;
	std::vector<Slot0> actions;

	std::ostream & info_output;
	std::ostream & error_output;

	unsigned int frame; ///< physics frame counter
	unsigned int displayframe; ///< display frame counter
	double clocktime; ///< elapsed wall clock time
	double target_time;
	const float timestep; ///< simulation time step

	PATHMANAGER pathmanager;
	SETTINGS settings;
	WINDOW_SDL window;
	GRAPHICS * graphics_interface;
	bool enableGL3;
	bool usingGL3;
	StringIdMap stringMap;
	EVENTSYSTEM_SDL eventsystem;
	ContentManager content;
	SOUND sound;
	AUTOUPDATE autoupdate;
	UPDATE_MANAGER carupdater;
	UPDATE_MANAGER trackupdater;
	std::map <std::string, FONT> fonts;
	std::string renderconfigfile;

	SCENENODE debugnode;
	TEXT_DRAWABLE fps_draw;
	TEXT_DRAWABLE profiling_text;

	std::vector <float> fps_track;
	int fps_position;
	float fps_min;
	float fps_max;

	bool multithreaded;
	bool profilingmode;
	bool debugmode;
	bool benchmode;
	bool dumpfps;
	bool pause;

	std::vector <EVENTSYSTEM_SDL::JOYSTICK> controlgrab_joystick_state;
	std::pair <int,int> controlgrab_mouse_coords;
	CARCONTROLMAP_LOCAL::CONTROL controlgrab_control;
	std::string controlgrab_page;
	std::string controlgrab_input;
	size_t controlgrab_id;
	bool controlgrab;

	CAMERA_FREE garage_camera;
	std::vector <std::string> cars_name;
	std::vector <std::string> cars_paint;
	std::vector <MATHVECTOR<float, 3> > cars_color_hsv;
	std::vector <std::string> cars_ai_type;
	std::vector <float> cars_ai_level;
	size_t car_edit_id;

	CAMERA * active_camera;
	std::pair <CAR *, CARCONTROLMAP_LOCAL> carcontrols_local;
	std::map <CAR *, int> cartimerids;
	std::list <CAR> cars;
	int race_laps;
	bool practice;

	btDefaultCollisionConfiguration collisionconfig;
	btCollisionDispatcher collisiondispatch;
	btDbvtBroadphase collisionbroadphase;
	btSequentialImpulseConstraintSolver collisionsolver;
	DynamicsDraw dynamicsdraw;
	DynamicsWorld dynamics;
	int dynamics_drawmode;

	PARTICLE_SYSTEM tire_smoke;
	unsigned int particle_timer;

	TRACKMAP trackmap;
	TRACK track;
	GUI gui;
	HUD hud;
	INPUTGRAPH inputgraph;
	LOADINGSCREEN loadingscreen;
	TIMER timer;
	REPLAY replay;
	AI ai;
	HTTP http;

	std::auto_ptr <FORCEFEEDBACK> forcefeedback;
	double ff_update_time;
};

#endif
