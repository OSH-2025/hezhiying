<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>鹤之影 (hezhiying) @USTC</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f2f5;
            margin: 0;
            padding: 20px;
            color: #333;
        }
        h1, h3 {
            text-align: center;
            color: #2c3e50;
        }
        h1 {
            font-size: 2.5em;
            margin-bottom: 0;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1);
        }
        h3 {
            font-size: 1.5em;
            color: #34495e;
        }
        .container {
            max-width: 900px;
            margin: 0 auto;
            background: #fff;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }
        .team-members ul {
            list-style: none;
            padding: 0;
            display: flex;
            justify-content: space-around;
            flex-wrap: wrap;
        }
        .team-members li {
            margin: 10px 0;
        }
        .team-members a {
            text-decoration: none;
            color: #2980b9;
            font-weight: bold;
            transition: color 0.3s;
        }
        .team-members a:hover {
            color: #e74c3c;
        }
        .progress-table {
            margin-top: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-bottom: 20px;
            display: none; /* 默认隐藏 */
        }
        table.show {
            display: table; /* 显示时覆盖 */
        }
        th, td {
            padding: 12px;
            text-align: left;
            border-bottom: 1px solid #ddd;
        }
        th {
            background-color: #3498db;
            color: white;
        }
        tr:nth-child(even) {
            background-color: #f9f9f9;
        }
        tr:hover {
            background-color: #ecf0f1;
        }
        .control-buttons {
            text-align: center;
            margin-bottom: 20px;
        }
        button {
            padding: 10px 20px;
            background-color: #3498db;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            margin: 0 10px;
            transition: background-color 0.3s;
        }
        button:hover {
            background-color: #2980b9;
        }
        .logo {
            text-align: center;
            margin-top: 20px;
        }
        .logo img {
            max-width: 300px;
            border-radius: 10px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }
        .logo a {
            text-decoration: none;
            color: #7f8c8d;
            font-style: italic;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>鹤之影 (hezhiying) @USTC</h1>
        <h3>OSH-2025-hezhiying @USTC</h3>

        <p style="text-align: center; color: #666;">
            This is the GitHub homepage for the "鹤之影" (hezhiying) project, developed by students from the University of Science and Technology of China (USTC) for the OSH-2025 course. The project focuses on exploring topics related to operating systems, particularly in the context of embedded systems and single-chip microcomputers.
        </p>

        <hr style="border: 0; height: 1px; background: #ddd; margin: 20px 0;">

        <div class="team-members">
            <h2>Team Members</h2>
            <ul>
                <li><a href="https://github.com/jianyingzhihe" target="_blank">位文康</a></li>
                <li><a href="https://github.com/ustcljh" target="_blank">罗嘉宏</a></li>
                <li><a href="https://github.com/crosaa" target="_blank">崔卓</a></li>
                <li><a href="https://github.com/EricGuoYanzhen" target="_blank">郭彦禛</a></li>
            </ul>
        </div>

        <div class="progress-table">
            <h2>Project Progress</h2>
            <div class="control-buttons">
                <button onclick="toggleTable()">Show/Hide Progress</button>
                <button onclick="window.scrollTo(0, document.body.scrollHeight)">To Bottom</button>
                <button onclick="window.scrollTo(0, 0)">To Top</button>
            </div>
            <table id="progressTable">
                <thead>
                    <tr>
                        <th>Stage</th>
                        <th>Task Details</th>
                        <th>Date</th>
                        <th>Progress</th>
                        <th>Notes</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>Topic Selection</td>
                        <td>We had in-depth discussions around the topic of the big assignment. Firstly, we discussed the selection of underlying related content, such as multiprocessor systems or file systems, and mentioned the recently completed Linux 101 project. We believed that it could be expanded on this basis, but some people prefer the direction of combining with AI. However, after discussion, we believe that AI belongs to the application layer and is not closely related to the operating system, and we are concerned about insufficient innovation. We have continued to consider previous year's titles and repo resources, but feel that simply copying them lacks novelty. We also propose the embedded direction, believing that it can be combined with previously learned knowledge and has a stronger relevance to operating system courses. In order to better advance the project, we have decided to conduct research on the topic discussed today and have another meeting on Wednesday to discuss it.</td>
                        <td>March 16</td>
                        <td>Done</td>
                        <td></td>
                    </tr>
                    <tr>
                        <td>Topic Selection</td>
                        <td>Discussed IoT fine-grained OS design, improving StarPU functionality (considered C/C++ over Rust due to limitations), and single-chip microcontroller design: run multiple programs on one chip, then N chips with N programs, finally N chips with >N programs.</td>
                        <td>March 19</td>
                        <td>Done</td>
                        <td></td>
                    </tr>
                </tbody>
            </table>
        </div>

        <div class="objectives">
            <h2>Project Objectives</h2>
            <p>The project aims to:</p>
            <ul>
                <li>Explore advanced operating system concepts in embedded systems.</li>
                <li>Develop a practical solution for single-chip microcomputer applications.</li>
                <li>Enhance skills in system design and implementation.</li>
            </ul>
        </div>

        <div class="logo">
            <h2>Group Logo</h2>
            <a href="./src/fufu.jpg" target="_blank">
                <img src="./src/fufu.jpg" alt="Group Logo">
            </a>
            <p><em>永世传颂，不休独舞，神爱世人，芙门永存！</em></p>
        </div>
    </div>

    <script>
        function toggleTable() {
            const table = document.getElementById('progressTable');
            table.classList.toggle('show');
        }
    </script>
</body>
</html>
