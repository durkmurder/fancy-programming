


Process::Process(const ustring &pCmdLine) : mCmdLine(pCmdLine)
{

}

Process::Process(const ustring &pCmdLine, AbstractLogger *logger) : Process(pCmdLine)
{
	
}


Process::~Process()
{
	
}

// Creates process and calls monitor method. Throws exception if fails to start new process, 
// if process is already running, logs info about it.
void Process::run()
{
	
}

void Process::terminate()
{

}

void Process::restart()
{

}

void Process::initInfo()
{

}

void Process::clearInfo()
{

}

int Process::processId() const
{
	
}


void Process::setEventCallback(std::function<void()> callback, const EventType evenType)
{
	
}

// Static function that calls when process terminates. It decides what to log and what callbacks to call.
void NTAPI Process::onTerminated(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	
	
	
}

ustring NTAPI Process::processCommandLine(void *handle)
{
	
}

void Process::setLogger(AbstractLogger *logger)
{
	
}

// Function that takes pID and starts to monitor selected process.
// Can be used in method run or separatly
void Process::monitor(const int pId)
{	
	
}

void Process::onEvent(const EventType type) const
{

}

bool Process::isRunning() const
{
	
}