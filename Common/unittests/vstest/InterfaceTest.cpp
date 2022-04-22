#include <System.h>
#include <stdio.h>
#include <System.Console.hpp>
#include <vector>
#include <algorithm>
using namespace std;

struct Temperature
{
	int Degrees;
	int Date;
};

/* https://docs.microsoft.com/zh-cn/dotnet/standard/events/how-to-implement-a-provider */
class TemperatureReporter;

class Unsubscriber : public IDisposable<Unsubscriber>
{
private:
	vector<shared_ptr<IObserver<TemperatureReporter, Temperature>>>* _observers;
	shared_ptr<IObserver<TemperatureReporter, Temperature>> _observer;

public:
	Unsubscriber(vector<shared_ptr<IObserver<TemperatureReporter, Temperature>>>* observers, shared_ptr<IObserver<TemperatureReporter, Temperature>> observer) :
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

class TemperatureMonitor : public IObservable<TemperatureMonitor, Temperature>
{
	vector<shared_ptr<IObserver<TemperatureReporter, Temperature>>> observers;
public:
	TemperatureMonitor() = default;
	shared_ptr<IDisposable<Unsubscriber>> Subscribe(shared_ptr<IObserver<TemperatureReporter, Temperature>> observer)
	{
		if (observers.end() == find(observers.begin(), observers.end(), observer))
		{
			observers.push_back(observer);
		}
		shared_ptr<Unsubscriber> p = make_shared<Unsubscriber>(&observers, observer);
		return static_pointer_cast<IDisposable<Unsubscriber>>(p);
	}

	void MonitorTemperatureTask()
	{
		printf("begin monitor temperature task...\n");
		// Create an array of sample data to mimic a temperature device.
		vector<int> temps = { 1, 3, 5, 7, 9 , 0};
		for (const auto& p : temps)
		{
			::Sleep(500);
			printf("...\n");
			if (0 == p)
			{
				for_each(observers.begin(), observers.end(), [](shared_ptr<IObserver<TemperatureReporter, Temperature>>& item)
				{
					VOIDRET_ASSERT(item.get());
					item->OnCompleted();
				});
				break;
			}
			for_each(observers.begin(), observers.end(), [&](shared_ptr<IObserver<TemperatureReporter, Temperature>>& item)
			{
				VOIDRET_ASSERT(item.get());
				Temperature temp{ p, -1 };
				item->OnNext(temp);
			});
		}
		printf("end monitor temperature task...\n");
	}
};

class TemperatureReporter : public IObserver<TemperatureReporter, Temperature>
{
private:
	bool first{ false };
	Temperature last{};
	//IDisposable<Unsubscriber> unsubscriber;
public:
	TemperatureReporter() = default;
	//void Subscribe(IObservable<Temperature> provider)
	//{
	//	unsubscriber = provider.Subscribe(this);
	//}

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
		Console::WriteLine("The temperature is ", value.Degrees, "°„C at ", value.Date);
		if (first)
		{
			last = value;
			first = false;
		}
		else
		{
			Console::WriteLine("   Change: ", value.Degrees - last.Degrees, "°„C in ", value.Date - last.Date);
		}
	}

	/*void Unsubscribe()
	{
		unsubscriber.Dispose();
	}*/
};

void SubscribTest()
{
	TemperatureMonitor monitor;
	auto unsubscriber = monitor.Subscribe(make_shared<IObserver<TemperatureReporter, Temperature>>());
	monitor.MonitorTemperatureTask();
}
class A : public IComparable<A>
{
public:
	A(int a) : val(a) {}
	int CompareTo(const A& other) const CRTP_OVERRIDE
	{
		return val > other.val ? 1 : 0;
	}
	int val;
};
int main()
{
	SubscribTest();
	return 0;
	IComparable<A>& a = static_cast<IComparable<A>&>(A(3));
	IComparable<A>& b = static_cast<IComparable<A>&>(A(4));
	printf("%d", a > b );
	return 0;
}