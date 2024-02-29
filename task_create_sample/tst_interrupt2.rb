#
# タスクに、外部から例外を注入する。
#
until task = Task.get("Task1"); Task.pass; end

while true
  sleep 5
  task.raise( RangeError.new("message here!") )
end
