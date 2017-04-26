// PX2Application.inl

//----------------------------------------------------------------------------
inline const Sizef &Application::GetBoostSize() const
{
	return mBoostInfo.BoostSize;
}
//----------------------------------------------------------------------------
inline const std::string &Application::GetBoostProjectName() const
{
	return mBoostInfo.ProjectName;
}
//----------------------------------------------------------------------------
inline const std::string &Application::GetProjectFilePath() const
{
	return mProjectFilePath;
}
//----------------------------------------------------------------------------
inline const Sizef &Application::GetScreenSize() const
{
	return mScreenSize;
}
//----------------------------------------------------------------------------
inline AppBoostInfo::PlayLogicMode Application::GetPlayLogicMode() const
{
	return mBoostInfo.ThePlayLogicMode;
}
//----------------------------------------------------------------------------
inline Canvas *Application::GetEngineCanvas()
{
	return mEngineCanvas;
}
//----------------------------------------------------------------------------
inline Application::PlayType Application::GetPlayType() const
{
	return mPlayType;
}
//----------------------------------------------------------------------------