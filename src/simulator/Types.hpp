#pragma once

using FinishTimestamp = long long;
using MachineID = unsigned;
using Assignation = std::tuple<FinishTimestamp,
			       OperationSP,
			       MachineID>;
