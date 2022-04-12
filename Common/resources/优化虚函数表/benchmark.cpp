/* https://quick-bench.com/q/lOC-EP2EJ7zpA3pbd0zchSXACxw */
#include <vector>

using DataBlock = std::vector<int>;
using DataType = int;
using Context = int;


class Expression {
public:
    virtual void update(Context* context,DataType &input) = 0;
};
template<typename ImplSelf>
class ExpressionCRTP : public Expression {
public:
    virtual void update(Context* context,DataType &input) override
    {
        static_cast<ImplSelf&>(*this).update1(context, input);
    }
};
/* VirtualCall */
class SumExpression: public Expression {
public:
	void update(Context* context,DataType &input) {
		(*(DataType*)context) += input;
	}
};
/* CRTP */
class SumExpressionCRTP /* final */: public ExpressionCRTP<SumExpressionCRTP>
{
public:
	void update1(Context* context,DataType &input) {
		(*(DataType*)context) += input;
	}
};
/* VirtualCall3 */
class SumExpressionV2 final: public Expression {
public:
	void update(Context* context,DataType &input) {
		(*(DataType*)context) += input;
	}
};
/* FunctionCall */
class SumExpressionNoVirtualCall {
public:
	void update(Context* context,DataType &input) {
		(*(DataType*)context) += input;
	}
};

static void VirtualCall(benchmark::State& state) {
  DataBlock input_block;
  for(int i = 0; i < 1024;++i) {
    input_block.push_back(i);
  }
  Expression *expression = new SumExpression();
  Context context;

  for (auto _ : state) {
    for(int i = 0;i < input_block.size();++i) {
      expression->update(&context, input_block[i]); // update
    }
  }
}
BENCHMARK(VirtualCall);

static void VirtualCall2(benchmark::State& state) {
  DataBlock input_block;
  for(int i = 0; i < 1024;++i) {
    input_block.push_back(i);
  }
  Expression *expression = new SumExpression();
  Context context;

  for (auto _ : state) {
    for(int i = 0;i < input_block.size();++i) {
      ((SumExpressionV2*)(expression))->update(&context, input_block[i]); // update
    }
  }
}
BENCHMARK(VirtualCall2);


static void FunctionCall(benchmark::State& state) {
  DataBlock input_block;
  for(int i = 0; i < 1024;++i) {
    input_block.push_back(i);
  }
  SumExpressionNoVirtualCall *expression = new SumExpressionNoVirtualCall();
  Context context;
  for (auto _ : state) {
      for(int i = 0;i < input_block.size();++i) {
        expression->update(&context, input_block[i]); // update
      }
  }
}
BENCHMARK(FunctionCall);

class IBase {
public:
    virtual void batch_update(Context* context,DataBlock &input_block) = 0;
};

template <typename T>
class Base: public IBase
{
public:
    void batch_update(Context* context,DataBlock &input_block)
    {
        T& derived = static_cast<T&>(*this);
        for(int i = 0;i < input_block.size(); ++i) {
        	derived.update(context, input_block[i]);
        }
    }
};
// ÕâÀïµÝ¹éÁË
class SumCRTP : public Base<SumCRTP> {
public:
	void update(Context* context,DataType &input) {
		(*(DataType*)context) += input;
	}
};
static void VirtualCall3(benchmark::State& state) {
  DataBlock input_block;
  for(int i = 0; i < 1024;++i) {
    input_block.push_back(i);
  }
  IBase *expression = new SumCRTP();
  Context context;
  for (auto _ : state) {
        expression->batch_update(&context, input_block); // update
  }
}
BENCHMARK(VirtualCall3);

static void VirtualCallCRTP(benchmark::State& state) {
  DataBlock input_block;
  for(int i = 0; i < 1024;++i) {
    input_block.push_back(i);
  }
  Expression *expression = new SumExpressionCRTP();
  Context context;

  for (auto _ : state) {
    for(int i = 0;i < input_block.size();++i) {
      expression->update(&context, input_block[i]); // update
      // ((SumExpressionCRTP*)(expression))->update(&context, input_block[i]);
    }
  }
}
BENCHMARK(VirtualCallCRTP);