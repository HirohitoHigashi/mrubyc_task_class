#
# タスクに、外部から例外を注入する。
#  受け側
#

Task.name = "Task1"

i = 0
while true
  begin
    puts "Task1: #{i+=1}"
    sleep 2
    puts "wakeup"

  rescue =>ex
    puts ex
  end
end
