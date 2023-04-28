# PianoPlay
Thoả mong muốn tự tạo game chơi piano có thể chơi bài hát mình muốn chứ không chỉ giới hạn ở những bài hát kinh điển có sẵn.

Mô phỏng game Piano Tiles trên điện thoại. Đây là một chủ đề có khá nhiều project mẫu, tuy nhiên những project hiện có chỉ chạy độc lập âm thanh và phím ấn, dù phím ấn nhanh hay chậm không hề ảnh hướng đến âm thanh, tạo cảm giác kém chân thực.
Phiên bản Piano Tiles này cải tiến có sự tương tác giữa âm thanh và việc người dùng ấn phím, ấn phím với nhịp độ nhanh, âm nhạc sẽ có tiết tấu nhanh tương ứng, chậm sẽ chậm tương ứng, như khi mình đánh piano thật vậy.
Có thể điều chỉnh tốc độ ở một mức cụ thể, hoặc challenging tăng dần tốc độ.
Chế độ autoplay linh hoạt cho phép tự chơi mỗi khi chỉ muốn nghe nhạc.
Thao tác pause/play bằng phím SPACE cho phép tạm dừng bài nhạc linh hoạt.
Chơi hết bài hát này sẽ tự động mở bài hát tiếp theo mà không ảnh hưởng đến điểm số. Nếu chơi thua vẫn có thể chơi tiếp chỗ đánh sai trong ván mới bằng phím SPACE, chứ không cần đánh lại bài từ đầu.
Giao diện từ điều chỉnh theo độ phân giải của từng máy tính.

Tạo bài hát mong muốn cho game: Đọc từ piano sheet, tự nghĩ cấu trúc để mã hoá tất cả các ký tự âm nhạc (chơi đồng thời 2 luồng khoá Sol cao và khoá Fa trầm, chơi nhiều nốt một lúc, trường độ, cao độ, dấu hoá, dấu lặng,...) thành một file text. Game sẽ đọc từ file này, với nhịp độ được tuỳ chỉnh, gán cho mỗi phím nhạc bằng một dãy các nốt nhạc tương ứng tại thời điểm ấn.
Xử lý đồng bộ hoá âm thanh với hình ảnh: căn chỉnh các khung thời gian chơi nốt với tốc độ chạy phím piano trên màn hình. Vì bản nhạc tự nhiên có độ phức tạp rất cao, cần phải xử lý cả những nốt nhạc dài kéo dài tận mấy phím piano, cả những nốt nhạc rất ngắn chỉ bằng 1/4 phím piano, thậm chí có những nốt nhạc lệch so với nhịp thông thường (bắt đầu khi đang ở giữa khoảng thời gian ấn phím). Đồng thời, còn phải mở nhạc trên nhiều kênh khác nhau để có thể chơi nhiều nốt cùng 1 lúc và xử lý dấu lặng.
