#include <System.h>
#include <stdio.h>
#include <System.Console.hpp>
#include <vector>
#include <algorithm>
using namespace std;
using namespace Interface;

struct Temperature
{
	int Degrees;
	int Date;
};

class TemperatureReporter;
class TemperatureMonitor;
class Unsubscriber;

using ITemperatureObserver = IObserver<TemperatureReporter, Temperature>;
using ITemperatureObservable = IObservable<TemperatureMonitor, Temperature>;
using IUnsubscriber = IDisposable<Unsubscriber>;

class Unsubscriber : public IUnsubscriber
{
private:
	WEAK_PTR(vector<WEAK_PTR(ITemperatureObserver)>) _observers;
	WEAK_PTR(ITemperatureObserver) _observer;

public:
	Unsubscriber(vector<WEAK_PTR(ITemperatureObserver)>* observers, WEAK_PTR(ITemperatureObserver) observer) :
		_observers(observers), _observer(observer)
	{
	}

	void Dispose() CRTP_OVERRIDE
	{
		VOIDRET_ASSERT(_observers && _observer);
		auto& it = find(_observers->begin(), _observers->end(), _observer);
		if (_observers->end() != it)
		{
			_observers->erase(it);
		}
	}
};
/* 如果从生产端和消费端都可以注册, 需要加锁(生产消费往往不在同一个线程) */
#define ENABLE_SUBSCRIBE_BOTH_SIZD
class TemperatureMonitor : public ITemperatureObservable
{
	vector<WEAK_PTR(ITemperatureObserver)> observers;
public:
	TemperatureMonitor() = default;
	IUnsubscriber* Subscribe(const ITemperatureObserver& observer) CRTP_OVERRIDE
	{
#ifndef ENABLE_SUBSCRIBE_BOTH_SIZD
		ITemperatureObserver* p = const_cast<ITemperatureObserver*>(&observer);
		if (observers.end() == find(observers.begin(), observers.end(), p))
		{
			observers.push_back(p);
		}
#else
		// lock
		ITemperatureObserver* p = const_cast<ITemperatureObserver*>(&observer);
		if (observers.end() == find(observers.begin(), observers.end(), p))
		{
			observers.push_back(p);
		}
		// unlock
#endif
		return new Unsubscriber(&observers, p);
	}

	void MonitorTemperatureTask()
	{
		printf("begin monitor temperature task...\n");
		// Create an array of sample data to mimic a temperature device.
		vector<int> temps = { 1, 3, 5, 7, 9 , 0};
		for (const auto& p : temps)
		{
			::Sleep(200);
			printf("...\n");
			if (0 == p)
			{
				for_each(observers.begin(), observers.end(), [&](WEAK_PTR(ITemperatureObserver) item)
				{
					item->OnCompleted();
				});
				break;
			}
			for_each(observers.begin(), observers.end(), [&](WEAK_PTR(ITemperatureObserver) item)
			{
				Temperature temp{ p, -1 };
				item->OnNext(temp);
			});
		}
		printf("end monitor temperature task...\n");
	}
};

class TemperatureReporter : public ITemperatureObserver
{
private:
	bool first{ false };
	Temperature last{};
	shared_ptr<IUnsubscriber> unsubscriber;
public:
	TemperatureReporter() = default;

	void Subscribe(ITemperatureObservable& provider)
	{
		 unsubscriber = shared_ptr<IUnsubscriber>(provider.Subscribe(*this));
	}
	void Unsubscribe()
	{
		unsubscriber->Dispose();
	}

	void OnCompleted() CRTP_OVERRIDE
	{
		Console::WriteLine("Additional temperature data will not be transmitted.");
	}
	void OnError(const std::exception& e) CRTP_OVERRIDE
	{
		Console::WriteLine(e.what());
	}
	inline void OnNext(const Temperature& value) CRTP_OVERRIDE
	{
		Console::WriteLine("The temperature is ", value.Degrees, "°C at ", value.Date);
		if (first)
		{
			last = value;
			first = false;
		}
		else
		{
			Console::WriteLine("   Change: ", value.Degrees - last.Degrees, "°C in ", value.Date - last.Date);
		}
	}
};

void SubscribTest()
{
	shared_ptr<ITemperatureObservable> monitor = make_shared<TemperatureMonitor>();
	shared_ptr<ITemperatureObserver> observer = make_shared<TemperatureReporter>();
	/* 1. 从消费端注册 */
	static_cast<TemperatureReporter*>(observer.get())->Subscribe(*monitor);
	static_cast<TemperatureMonitor*>(monitor.get())->MonitorTemperatureTask();
	printf("------------\n");
	static_cast<TemperatureReporter*>(observer.get())->Unsubscribe();
	static_cast<TemperatureMonitor*>(monitor.get())->MonitorTemperatureTask();

	/* 2. 从生产端注册 */
	shared_ptr<IUnsubscriber> unsubscriber = shared_ptr<IUnsubscriber>(monitor->Subscribe(*observer));
	static_cast<TemperatureMonitor*>(monitor.get())->MonitorTemperatureTask();
	printf("------------\n");
	unsubscriber->Dispose();
	static_cast<TemperatureMonitor*>(monitor.get())->MonitorTemperatureTask();
}
int main()
{
#ifdef _DEBUG
	MEMORYLEAK_ASSERT;
#endif
	SubscribTest();
	return 0;
}